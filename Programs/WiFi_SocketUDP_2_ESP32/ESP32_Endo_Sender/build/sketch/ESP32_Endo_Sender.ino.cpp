#include <Arduino.h>
#line 1 "/Users/tecla/Documents/Robotica/practiques/Projecte 2 Surgical Robotics/VScode_Arduino_ESP32/Programs/WiFi_SocketUDP_2_ESP32/ESP32_Endo_Sender/ESP32_Endo_Sender.ino"
#include <WiFi.h>
#include <WiFiUdp.h>
#include "MPU9250.h"
#include <Wire.h> //needed for I2C to read IMU
#include <ArduinoJson.h>

// Device ID
const char *deviceId = "G7_Endo";

// Wi-Fi credentials
const char *ssid = "MIWIFI_2C34";      
const char *password = "RSYZG2RC";   

// UDP settings
IPAddress receiverESP32IP(192, 168, 1, 144); // IP address of the receiver ESP32 (gripper)
IPAddress receiverComputerIP(192, 168, 1, 130); // IP address of your computer (computer)
const int udpPort = 12345;

// UDP object
WiFiUDP udp;

// MPU-9250 object
MPU9250 mpu;
// Orientation data
float roll = 0.0, pitch = 0.0, yaw = 0.0;

#line 27 "/Users/tecla/Documents/Robotica/practiques/Projecte 2 Surgical Robotics/VScode_Arduino_ESP32/Programs/WiFi_SocketUDP_2_ESP32/ESP32_Endo_Sender/ESP32_Endo_Sender.ino"
void connectToWiFi();
#line 40 "/Users/tecla/Documents/Robotica/practiques/Projecte 2 Surgical Robotics/VScode_Arduino_ESP32/Programs/WiFi_SocketUDP_2_ESP32/ESP32_Endo_Sender/ESP32_Endo_Sender.ino"
void updateOrientation();
#line 48 "/Users/tecla/Documents/Robotica/practiques/Projecte 2 Surgical Robotics/VScode_Arduino_ESP32/Programs/WiFi_SocketUDP_2_ESP32/ESP32_Endo_Sender/ESP32_Endo_Sender.ino"
void sendOrientationUDP();
#line 73 "/Users/tecla/Documents/Robotica/practiques/Projecte 2 Surgical Robotics/VScode_Arduino_ESP32/Programs/WiFi_SocketUDP_2_ESP32/ESP32_Endo_Sender/ESP32_Endo_Sender.ino"
void setup();
#line 96 "/Users/tecla/Documents/Robotica/practiques/Projecte 2 Surgical Robotics/VScode_Arduino_ESP32/Programs/WiFi_SocketUDP_2_ESP32/ESP32_Endo_Sender/ESP32_Endo_Sender.ino"
void loop();
#line 27 "/Users/tecla/Documents/Robotica/practiques/Projecte 2 Surgical Robotics/VScode_Arduino_ESP32/Programs/WiFi_SocketUDP_2_ESP32/ESP32_Endo_Sender/ESP32_Endo_Sender.ino"
void connectToWiFi() {
  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi connected!");
  Serial.println("IP Address: " + WiFi.localIP().toString());
  Serial.print("ESP32 MAC Address: ");
  Serial.println(WiFi.macAddress());
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
  Wire.begin();//needed for I2C to read IMU
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

