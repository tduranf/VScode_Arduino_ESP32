from robodk.robolink import *
from robodk.robomath import *
import time
import math
import tkinter as tk
import threading
import socket
import json

# Constants
UDP_IP = "0.0.0.0"
UDP_PORT = 12345
BUFFER_SIZE = 1024
ROBOT_NAME = 'UR5e'
ZERO_YAW_TOOL = 0
ZERO_YAW_GRIPPER = 0
READ_INTERVAL_S = 0.01

latest_rpy_endowrist = None
endowrist_data_lock = threading.Lock()# semaphor to manage data from 2 threads

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))
print(f"Listening on {UDP_IP}:{UDP_PORT}")

# Initialize RoboDK
def initialize_robodk():
    RDK = Robolink()
    robot = RDK.Item(ROBOT_NAME)
    base = RDK.Item(f'{ROBOT_NAME} Base')
    endowrist = RDK.Item('Endowrist')
    gripper = RDK.Item('Gripper')
    needle = RDK.Item('Needle')
    Init_target = RDK.Item('Init')
    robot.setPoseFrame(base)
    robot.setPoseTool(endowrist)
    gripper_init = TxyzRxyz_2_Pose([0, 5, -105, 0, 0, 0])
    gripper.setParent(endowrist)
    gripper.setPose(gripper_init)
    needle_init = TxyzRxyz_2_Pose([0, 0, 0, 0, 0, 0])
    needle.setParent(gripper)
    needle.setPose(needle_init)
    robot.MoveL(Init_target)
    robot.setSpeed(5)
    return RDK, robot, base, endowrist, gripper, needle, Init_target

# Transformation Endowrist to base
def endowrist2base_orientation(roll, pitch, yaw):
    roll2 = (roll + 90) % 360
    pitch2 = pitch % 360
    yaw2 = yaw % 360
    return roll2, pitch2, yaw2

# Function to update the label with text
def update_text_label(label, tool_orientation, gripper_orientation, status_message, torque_values):
    full_text = f"Tool orientation: {tool_orientation}\nGripper orientation: {gripper_orientation}\n{status_message}\n{torque_values}"
    label.after(0, lambda: label.config(text=full_text))

# Function to read UDP data and update the global variable
def read_rpy_endowrist():
    global latest_rpy_endowrist, endowrist_data_lock
    while True:
        try:
            data, addr = sock.recvfrom(BUFFER_SIZE)
            try:
                orientation = json.loads(data.decode())
                device_id = orientation.get("device")
                if device_id == "G7_Endo":
                    with endowrist_data_lock:
                        latest_rpy_endowrist = orientation
            except json.JSONDecodeError:
                print("Error decoding JSON data")
        except socket.error as e:
            print(f"Socket error in UDP reader: {e}")
            break
        time.sleep(0.001)

# Function to process the latest UDP data and move the robot
def move_robot(robot, gripper, base, text_label):
    global ZERO_YAW_TOOL, ZERO_YAW_GRIPPER, latest_rpy_data, endowrist_data_lock
    status_message = ""
    torque_values = ""
    while True:
        with endowrist_data_lock:
            current_data = latest_rpy_endowrist
        if current_data:
            e_roll = current_data.get("roll")
            e_pitch = current_data.get("pitch")
            e_yaw = current_data.get("yaw")
            endowrist_orientation_raw = f"R={round((e_roll+90)%360)} P={round(e_pitch)} W={round(e_yaw)}"

            # Move Endowrist and gripper
            endo_roll, endo_pitch, endo_yaw = endowrist2base_orientation(e_roll, e_pitch, e_yaw)
            R_t, P_t, W_t = map(math.radians, [endo_roll, endo_pitch, endo_yaw])
            robot_pose = robot.Pose()
            Xr, Yr, Zr, rr, pr, yr = Pose_2_TxyzRxyz(robot_pose)
            endowrist_pose = transl(Xr, Yr, Zr) * rotz(math.radians(ZERO_YAW_TOOL)) * rotz(W_t) * roty(P_t) * rotx(R_t)

            if robot.MoveL_Test(robot.Joints(), endowrist_pose) == 0:
                robot.MoveL(endowrist_pose, True)
                endowrist_orientation = f"R={round(endo_roll)} P={round(endo_pitch)} W={round((endo_yaw+ZERO_YAW_TOOL)%360)}"
                status_message = ""
            else:
                endowrist_orientation = f"R={round(endo_roll)} P={round(endo_pitch)} W={round((endo_yaw+ZERO_YAW_TOOL)%360)}"
                status_message = "Robot cannot reach the position"

            gripper_pose = gripper.Pose()
            Xg, Yg, Zg, rg, pg, yg = Pose_2_TxyzRxyz(gripper_pose)
            gripper_pose_new = transl(Xg, Yg, Zg) * rotz(math.radians(ZERO_YAW_GRIPPER))
            gripper.setPose(gripper_pose_new)
            gripper_orientation = f"R={round((e_roll+90)%360)} P={round(e_pitch)} W={round(e_yaw)}"

            update_text_label(text_label, endowrist_orientation, gripper_orientation, status_message, torque_values)

        time.sleep(READ_INTERVAL_S)

# Main function
def main():
    global root, ZERO_YAW_TOOL, ZERO_YAW_GRIPPER, robot, gripper, base, text_label

    RDK, robot, base, endowrist, gripper, needle, Init_target = initialize_robodk()

    root = tk.Tk()
    root.title("Suture Process")
    text_label = tk.Label(root, text="", wraplength=300)
    text_label.pack(padx=20, pady=20)

    # Add sliders for ZERO_YAW_TOOL and ZERO_YAW_GRIPPER
    tool_yaw_slider = tk.Scale(root, from_=-180, to=180, orient=tk.HORIZONTAL, label="Tool Yaw",
                                    command=lambda value: set_zero_yaw_tool(float(value)), length=200)
    tool_yaw_slider.set(ZERO_YAW_TOOL)
    tool_yaw_slider.pack()

    gripper_yaw_slider = tk.Scale(root, from_=-180, to=180, orient=tk.HORIZONTAL, label="Gripper Yaw",
                                        command=lambda value: set_zero_yaw_gripper(float(value)), length=200)
    gripper_yaw_slider.set(ZERO_YAW_GRIPPER)
    gripper_yaw_slider.pack()

    # Start the UDP reading thread
    udp_thread = threading.Thread(target=read_rpy_endowrist)
    udp_thread.daemon = True
    udp_thread.start()

    # Start the robot movement thread
    robot_thread = threading.Thread(target=move_robot, args=(robot, gripper, base, text_label))
    robot_thread.daemon = True
    robot_thread.start()

    root.mainloop()
    # Clean up after main loop
    sock.close()
    print("Program CLOSED")

# Update functions for sliders
def set_zero_yaw_tool(value):
    global ZERO_YAW_TOOL
    ZERO_YAW_TOOL = float(value)

def set_zero_yaw_gripper(value):
    global ZERO_YAW_GRIPPER
    ZERO_YAW_GRIPPER = float(value)

if __name__ == "__main__":
    main()