import serial
import keyboard

# Configuració del port Bluetooth
port = "COM4"  # Canvia'l pel teu port COM
baudrate = 115200

try:
    ser = serial.Serial(port, baudrate)
    while True:
        if keyboard.is_pressed('q'):  # Comprova si s'ha premut la tecla 'q'
            print("Tecla 'q' premuda. Sortint del programa.")
            break  # Surt del bucle si s'ha premut 'q'

        try:
            linea = ser.readline().decode("utf-8").rstrip()
            parts = linea.split(",")
            roll = float(parts[0])
            pitch = float(parts[1])
            yaw = float(parts[2])
            print(f"Roll: {roll}, Pitch: {pitch}, Yaw: {yaw}")

        except serial.SerialException as e:
            print(f"Error de lectura serial: {e}")
            break
        except UnicodeDecodeError as e:
            print(f"Error de decodificació: {linea} - {e}")
            break

except serial.SerialException as e:
    print(f"Error de connexió serial: {e}")