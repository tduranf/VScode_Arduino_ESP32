#include <Arduino.h>
<<<<<<< HEAD
#line 1 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\01_Eines_HW_SW\\03_SW\\11_Arduino_VScode\\VScode_Arduino_ESP32\\Programs\\WiFi_SocketUDP_2_ESP32\\ESP32_Sender\\ESP32_Sender.ino"
=======
#line 1 "C:\\Users\\manel.puig\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\Classes_PROJ\\TP\\VScode_Arduino_ESP32\\Programs\\WiFi_SocketUDP_2_ESP32\\ESP32_Sender\\ESP32_Sender.ino"
>>>>>>> 729aa31091f0a7e69d49d7629076aab36507433c
#include <WiFi.h>
#include <Wire.h>
#include <WiFiUdp.h>
#include "MPU9250.h"
#include <ArduinoJson.h>

// Device ID
const char *deviceId = "ESP32_1";

// Wi-Fi credentials
const char *ssid = "Robotics_UB";
const char *password = "rUBot_xx";

// UDP settings
IPAddress receiverESP32IP(192, 168, 0, 161); // IP address of the receiver ESP32 - CHANGE THIS!
IPAddress receiverComputerIP(192, 168, 0, 191); // IP address of your computer - CHANGE THIS!
const int udpPort = 12345;

// UDP object
WiFiUDP udp;

// MPU-9250 object
MPU9250 mpu;
// Orientation data
float roll = 0.0, pitch = 0.0, yaw = 0.0;

<<<<<<< HEAD
#line 27 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\01_Eines_HW_SW\\03_SW\\11_Arduino_VScode\\VScode_Arduino_ESP32\\Programs\\WiFi_SocketUDP_2_ESP32\\ESP32_Sender\\ESP32_Sender.ino"
void connectToWiFi();
#line 38 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\01_Eines_HW_SW\\03_SW\\11_Arduino_VScode\\VScode_Arduino_ESP32\\Programs\\WiFi_SocketUDP_2_ESP32\\ESP32_Sender\\ESP32_Sender.ino"
void updateOrientation();
#line 46 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\01_Eines_HW_SW\\03_SW\\11_Arduino_VScode\\VScode_Arduino_ESP32\\Programs\\WiFi_SocketUDP_2_ESP32\\ESP32_Sender\\ESP32_Sender.ino"
void sendOrientationUDP();
#line 71 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\01_Eines_HW_SW\\03_SW\\11_Arduino_VScode\\VScode_Arduino_ESP32\\Programs\\WiFi_SocketUDP_2_ESP32\\ESP32_Sender\\ESP32_Sender.ino"
void setup();
#line 94 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\01_Eines_HW_SW\\03_SW\\11_Arduino_VScode\\VScode_Arduino_ESP32\\Programs\\WiFi_SocketUDP_2_ESP32\\ESP32_Sender\\ESP32_Sender.ino"
void loop();
#line 27 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\01_Eines_HW_SW\\03_SW\\11_Arduino_VScode\\VScode_Arduino_ESP32\\Programs\\WiFi_SocketUDP_2_ESP32\\ESP32_Sender\\ESP32_Sender.ino"
=======
>>>>>>> 729aa31091f0a7e69d49d7629076aab36507433c
void connectToWiFi() {
  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi connected!");
  Serial.println("IP Address: " + WiFi.localIP().toString());
}

void updateOrientation() {
  if (mpu.update()) {
    yaw = -mpu.getYaw();
    pitch = -mpu.getPitch();
    roll = mpu.getRoll();
  }
}

void sendOrientationUDP() {
  StaticJsonDocument<256> doc;
  doc["device"] = deviceId;
  doc["roll"] = roll;
  doc["pitch"] = pitch;
  doc["yaw"] = yaw;

  char jsonBuffer[512];
  size_t bytes = serializeJson(doc, jsonBuffer);
    if (bytes == 0){
        Serial.println(F("Serialization Failed"));
        return;
    }

  // Send to ESP32
  udp.beginPacket(receiverESP32IP, udpPort);
  udp.write((const uint8_t*)jsonBuffer, bytes); // Cast to const uint8_t*
  udp.endPacket();

  // Send to Computer
  udp.beginPacket(receiverComputerIP, udpPort);
  udp.write((const uint8_t*)jsonBuffer, bytes); // Cast to const uint8_t*
  udp.endPacket();
}

void setup() {
  Serial.begin(115200);
  Wire.begin();
  delay(2000);

  // Inicialitza el MPU-9250
  if (!mpu.setup(0x68)) {
    while (1) {
      Serial.println("MPU connection failed. Please check your connection with `connection_check` example.");
      delay(5000);
    }
  }
  Serial.println("MPU connected");
  delay(2000);

  // Connecta a la xarxa Wi-Fi
  connectToWiFi();

  // Comença UDP
  udp.begin(udpPort);
  Serial.println("UDP initialized.");
}

void loop() {
  updateOrientation(); // Actualitza les dades del sensor
  sendOrientationUDP(); // Envia les dades al receptor via UDP
  delay(10); // Ajusta la freqüència d'enviament si cal
}

