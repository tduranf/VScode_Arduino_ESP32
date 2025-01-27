# **Programs**

The main objective is to read data from an IMU sensor and send this data to a main computer for its real-time processing.

**Program to read data from an IMU sensor**

A first simple program is created to read IMU MPU9250 sensor. We can use the libraries:
- SparkFunMPU9250-DMP (IMU_read1.ino)
- MPU9250 by hideakitai (IMU_read2.ino)

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
- Short Bursts of Data: Designed for unfrequent data transfers, such as sensor readings, notifications, and control signals.
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

Assigning fixed IPs and hostnames to each ESP32 prevents IP conflicts and simplifies device management, making it easier to identify and access each device consistently. This is especially useful in a lab with multiple devices.

A First program has been made to assign IP address and hostname to an ESP32: WiFi_IP_hostname.ino

In this program we have created a configureNetwork() function that we can use in applications where we are using several ESP32s.

The main objective is to read an IMU sensor and send data to a local computer to make postprocessing.

The ESP32 offers different interesting options:
- Webserver 
- Socket UDP

### **2.1. ESP32 webserver**

Publishes IMU data in real-time to a web server located in ESP32 

Programs:
- WiFi_WebServer_IMU1.ino: Uses SparkFun IMU library
- WiFi_WebServer_IMU2.ino: Uses mpu9250.h IMU library


### **2.2. Socket UDP**

This program reads orientation data (roll, pitch, yaw) from an MPU-9250 sensor connected to an ESP32 and sends this data over a Wi-Fi network using UDP (User Datagram Protocol) to a specified IP address and port.

This is useful for applications where you need to receive sensor data on a computer or other device in real-time. UDP is chosen for its speed and low overhead, making it suitable for streaming data, even though it doesn't guarantee delivery like TCP.

Programs: 
- ESP32 sends IMU data to computer (WiFi_SocketUDP_IMU.ino)
- ESP32_1 and ESP32_2 sends IMU data to computer (WiFi_SocketUDP_2IMU.ino)
- ESP32_1 sends IMU data to ESP32_2 and Computer (ESP32_Sender.ino)
- ESP32_2 receives IMU data from ESP32_1 (ESP32_Receiver.ino)


