# Setup and Install

Bibliography:
- Video Tutorial: https://www.youtube.com/watch?v=4WNsIj_7iqA
- https://www.arduino.cc/en/software
- https://docs.espressif.com/projects/arduino-esp32/en/latest/getting_started.html
- Arduino drivers: https://www.silabs.com/developer-tools/usb-to-uart-bridge-vcp-drivers?tab=downloads


## **1. Install drivers**

Sometimes you have not installed Arduino USB drivers in your PC:

- Go to "Arduino drivers's link and Download the drivers folder to “Descargas”. Unzip the downloaded driver.
- connect your ESP32 on USB port
- Open “Administrador de dispositivos”
- Right click to the port with warnings
- Select “actualizar driver” 
- Select the location of the downloaded driver and install
- You will have to restart the computer

## **2. Install Arduino extension in VScode**

Select the extension: "Arduino Community Edition" (vscode-arduino)

Select the Settings from this extension and verify:
- Arduino: Disable Testing Open (SELECTED)
- Arduino: Enable USBDetection (SELECTED)
- Arduino: Use Arduino CLI (SELECTED)

With these options the VScode will automatically configure the Arduino project


## **4. Create a first program**

To creata a first program, you have to create a VScode project folder:

- Create a fist Arduino folder (i.e. blink) and file (i.e. blink.ino) with the same name and extension "ino"
- Delete (if exists) "arduino.json" file
- Open the Command Palette (Ctrl+Shift+P) and run "Arduino: Initialize" to create a new arduino.json file
- Specify Board: ESP32 Dev Module as a generic ESP32 board
- Specify the port (usually COM3: Silicon Labs CP210x USB) in low bar menu
- In this arduino.json file add: "output": "./Programs/blink/build" (modify to the program path)
- Verify the Selected program in Bottom menu bar is the correct one.
- this will make compilation files to be available for future project modifications with less compilation time
- If you find errors in include lines, fix this errors selecting "Disable squiggles errors" (this will be added in "settings.json")
- Now you can upload (it compiles first and uploads later)
- The compiler is selected automatically on "Arduino"

For successive program uploads:
- Modify the program
- modify the arduino.json
- Initialyse again with Crtl+shift+P --> Initialize
- A new [Starting] line will start
- Perhaps ESP32 reset will be needed
