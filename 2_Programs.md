# **Programs**

The main objective is to read data from an IMU sensor and send this data to a main computer for its real-time processing.

**Program to read data from an IMU sensor**

A first simple program is created to read IMU MPU9250 sensor. We can use the libraries:
- SparkFunMPU9250-DMP (IMU_read1.ino)
. MPU9250 by hideakitai (IMU_read2.ino)

We propose you some sample programs to review the communication performances using:
- Bluetooth 
- WiFi

## **1. Bluetooth communication**

The ESP32 offers both Classic Bluetooth (BR/EDR) and Bluetooth Low Energy (BLE).

**Classic Bluetooth** (BR/EDR):

- Higher Data Rates: Designed for continuous data streaming
- Higher Power Consumption
- Range: Generally 10-30m
- Use Cases: Audio streaming (A2DP), serial communication (SPP), and other applications requiring continuous data transfer.

**Bluetooth Low Energy** (BLE):

- Low Power Consumption
- Lower Data Rates: Not ideal for continuous high-bandwidth data streaming.
- Short Bursts of Data: Designed for infrequent data transfers, such as sensor readings, notifications, and control signals.
- Range: Can achieve comparable or even better range than Classic Bluetooth 
- Use Cases: Wearables, IoT devices, and other applications where low power consumption is crucial.

We have mada a very simple BLE test program: 

**BLE_test1.ino**:
- Read input from monitor and write to Mobile
- Read input brom Mobile and write to Serial Monitor

**BLE_test2.ino**:
- Send data to Mobile
- Send data to Serial Monitor

We have made some modifications to the initial program to send IMU data with BLE: **BLE_IMU_send2.ino**
You need to install "Serial Bluetooth Terminal" App in your mobile phone.

## **2. WiFi communication**

We have made a program: WebServer_IMU_send.ino

The ESP32 offers different options:
- Publish IMU data in a web server located in ESP32
- Use websockets

### **2.1. ESP32 webserver**

Install libraries:
- ESPAsyncTCP by me-no-dev
- ArduinoJson


This program makes an ESP32 act as a web server that displays real-time orientation data (Roll, Pitch, Yaw) from an MPU-9250 IMU sensor.

- Includes and Setup: Includes necessary libraries (WiFi, IMU, JSON)
- sets up Wi-Fi credentials, initializes the IMU, and starts a web server.
- IMU Data Acquisition: Reads orientation data (RPY) from the IMU using its interrupt pin to trigger readings only when new data is available.
- Web Server Handling:
- Listens for incoming web requests.
If a request for /data is received, it sends the current RPY values as a JSON object.
For any other request (typically the main page request), it sends a simple HTML page that uses JavaScript and fetch to periodically request the /data endpoint and update the displayed values. This has been changed to a meta refresh tag to reduce the load on the ESP32.
Loop: Continuously updates the orientation data from the IMU and handles incoming web requests.
In essence, it's a simple web-based dashboard that shows the orientation of the IMU in real time. It's using a basic polling method (meta refresh) to update the data on the webpage. For more demanding real-time applications, WebSockets would be a better choice, but for simple visualization, this approach is sufficient and more efficient for the ESP32 in terms of processing power.

Open the Command Palette (Ctrl+Shift+P or Cmd+Shift+P).
Type "Arduino: Library Manager" and select it.
In the Library Manager, search for "ArduinoJson".
Select the latest version and click Install.


