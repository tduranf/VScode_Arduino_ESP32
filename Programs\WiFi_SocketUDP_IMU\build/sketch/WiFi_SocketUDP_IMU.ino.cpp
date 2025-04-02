#include <Arduino.h>
#line 1 "/Users/tecla/Documents/Robotica/practiques/Projecte 2 Surgical Robotics/VScode_Arduino_ESP32/Programs/WiFi_SocketUDP_IMU/WiFi_SocketUDP_IMU.ino"
#include <WiFi.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include "MPU9250.h"

// Wi-Fi credentials
const char *ssid = "MiFibra-E270";
const char *password = "Pvep6Pcu";

// MPU-9250 object
MPU9250 mpu;

// UDP object
WiFiUDP udp;

// IP i port del receptor
const char *udpAddress = "192.168.1.104"; // Canvia-ho per la IP del teu ordinador
const int udpPort = 12345;               // Port del receptor

// Orientation data
float roll = 0.0, pitch = 0.0, yaw = 0.0;

#line 23 "/Users/tecla/Documents/Robotica/practiques/Projecte 2 Surgical Robotics/VScode_Arduino_ESP32/Programs/WiFi_SocketUDP_IMU/WiFi_SocketUDP_IMU.ino"
void connectToWiFi();
#line 36 "/Users/tecla/Documents/Robotica/practiques/Projecte 2 Surgical Robotics/VScode_Arduino_ESP32/Programs/WiFi_SocketUDP_IMU/WiFi_SocketUDP_IMU.ino"
void updateOrientation();
#line 44 "/Users/tecla/Documents/Robotica/practiques/Projecte 2 Surgical Robotics/VScode_Arduino_ESP32/Programs/WiFi_SocketUDP_IMU/WiFi_SocketUDP_IMU.ino"
void sendOrientationUDP();
#line 59 "/Users/tecla/Documents/Robotica/practiques/Projecte 2 Surgical Robotics/VScode_Arduino_ESP32/Programs/WiFi_SocketUDP_IMU/WiFi_SocketUDP_IMU.ino"
void setup();
#line 82 "/Users/tecla/Documents/Robotica/practiques/Projecte 2 Surgical Robotics/VScode_Arduino_ESP32/Programs/WiFi_SocketUDP_IMU/WiFi_SocketUDP_IMU.ino"
void loop();
#line 23 "/Users/tecla/Documents/Robotica/practiques/Projecte 2 Surgical Robotics/VScode_Arduino_ESP32/Programs/WiFi_SocketUDP_IMU/WiFi_SocketUDP_IMU.ino"
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
  // Prepara les dades a enviar en format JSON
  String json = "{";
  json += "\"roll\":" + String(roll, 2) + ",";
  json += "\"pitch\":" + String(pitch, 2) + ",";
  json += "\"yaw\":" + String(yaw, 2);
  json += "}";
  // Print to Serial Monitor
  //Serial.println("Sending data: " + json);
  // Envia les dades via UDP
  udp.beginPacket(udpAddress, udpPort);
  udp.print(json);
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

