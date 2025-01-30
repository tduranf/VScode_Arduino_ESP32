import pygame
import serial
import matplotlib.pyplot as plt

# Configuració del port Bluetooth (igual que abans)
port = "nom_del_port_bluetooth"
baudrate = 115200

# Inicialització de pygame (igual que abans)
pygame.init()
screen = pygame.display.set_mode((400, 300))

# Inicialització de la gràfica (amb llistes en lloc de deque)
plt.ion()
fig, ax = plt.subplots()
line_roll, = ax.plot([], [], 'r', label='Roll')
line_pitch, = ax.plot([], [], 'g', label='Pitch')
line_yaw, = ax.plot([], [], 'b', label='Yaw')
ax.set_xlim(0, 100)
ax.set_ylim(-180, 180)
ax.legend()
roll_data = []
pitch_data = []
yaw_data = []

def main():
    try:
        ser = serial.Serial(port, baudrate)
        while True:
            linea = ser.readline().decode("utf-8").rstrip()
            if linea.startswith("Roll, Pitch, Yaw:"):
                try:
                    parts = linea.split(":")[1].split(",")
                    roll = float(parts[0].strip())
                    pitch = float(parts[1].strip())
                    yaw = float(parts[2].strip())

                    # Actualitzar dades i gràfica (amb llistes)
                    roll_data.append(roll)
                    pitch_data.append(pitch)
                    yaw_data.append(yaw)

                    # Limit de la longitud de les dades per a la gràfica (opcional)
                    max_len = 100  # Ajusta aquest valor segons sigui necessari
                    if len(roll_data) > max_len:
                        roll_data = roll_data[-max_len:]
                        pitch_data = pitch_data[-max_len:]
                        yaw_data = yaw_data[-max_len:]

                    line_roll.set_ydata(roll_data)
                    line_pitch.set_ydata(pitch_data)
                    line_yaw.set_ydata(yaw_data)

                    ax.relim()
                    ax.autoscale_view()
                    fig.canvas.flush()

                    # Visualitzar columnes RGB (igual que abans)
                    max_height = 200
                    roll_height = int(max_height * (roll + 180) / 360)
                    pitch_height = int(max_height * (pitch + 180) / 360)
                    yaw_height = int(max_height * (yaw + 180) / 360)

                    screen.fill((0, 0, 0))

                    pygame.draw.rect(screen, (255, 0, 0), (50, 250 - roll_height, 50, roll_height))
                    pygame.draw.rect(screen, (0, 255, 0), (150, 250 - pitch_height, 50, pitch_height))
                    pygame.draw.rect(screen, (0, 0, 255), (250, 250 - yaw_height, 50, yaw_height))

                    pygame.display.flip()

                except ValueError:
                    print("Dades no vàlides:", linea)

    except serial.SerialException as e:
        print(f"Error de connexió serial: {e}")
    except Exception as e:
        print(f"Un altre error: {e}")

if __name__ == "__main__":
    main()
    