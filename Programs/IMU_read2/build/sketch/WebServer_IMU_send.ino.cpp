#include <Arduino.h>
#line 1 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\01_Eines_HW_SW\\03_SW\\11_Arduino_VScode\\ESP32_Tutorial\\VScode_Arduino_ESP32\\Programs\\WebServer_IMU_send\\WebServer_IMU_send.ino"
#include <Wire.h>
#include "MPU9250.h"
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h> // Make sure to install this library

MPU9250 mpu;
WebServer server(80); // Create web server object on port 80

// Wi-Fi credentials
const char* ssid = "STRONG_ATRIA_AY4U_2.4";
const char* password = "3HCG3ykAUc";

#line 14 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\01_Eines_HW_SW\\03_SW\\11_Arduino_VScode\\ESP32_Tutorial\\VScode_Arduino_ESP32\\Programs\\WebServer_IMU_send\\WebServer_IMU_send.ino"
void handleRoot();
#line 18 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\01_Eines_HW_SW\\03_SW\\11_Arduino_VScode\\ESP32_Tutorial\\VScode_Arduino_ESP32\\Programs\\WebServer_IMU_send\\WebServer_IMU_send.ino"
void handleIMUData();
#line 39 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\01_Eines_HW_SW\\03_SW\\11_Arduino_VScode\\ESP32_Tutorial\\VScode_Arduino_ESP32\\Programs\\WebServer_IMU_send\\WebServer_IMU_send.ino"
void setup();
#line 70 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\01_Eines_HW_SW\\03_SW\\11_Arduino_VScode\\ESP32_Tutorial\\VScode_Arduino_ESP32\\Programs\\WebServer_IMU_send\\WebServer_IMU_send.ino"
void loop();
#line 14 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\01_Eines_HW_SW\\03_SW\\11_Arduino_VScode\\ESP32_Tutorial\\VScode_Arduino_ESP32\\Programs\\WebServer_IMU_send\\WebServer_IMU_send.ino"
void handleRoot() {
  server.send(200, "text/plain", "ESP32 IMU Data Server");
}

void handleIMUData() {
  if (mpu.update()) {
    float yaw = -mpu.getYaw();
    float pitch = -mpu.getPitch();
    float roll = mpu.getRoll();

    // Create JSON document
    StaticJsonDocument<200> doc;
    doc["roll"] = roll;
    doc["pitch"] = pitch;
    doc["yaw"] = yaw;

    String json;
    serializeJson(doc, json);

    server.send(200, "application/json", json);
  } else {
    server.send(500, "text/plain", "Error reading IMU data");
  }
}

void setup() {
  Serial.begin(115200);
  Wire.begin();
  delay(2000);

  if (!mpu.setup(0x68)) {
    while (1) {
      Serial.println("MPU connection failed.");
      delay(5000);
    }
  }
  Serial.println("MPU connected");

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to IP: ");
  Serial.println(WiFi.localIP());

  // Set up web server routes
  server.on("/", handleRoot);
  server.on("/imu", handleIMUData); // Endpoint for IMU data
  server.begin();
  Serial.println("Web server started");
}

void loop() {
  server.handleClient(); // Handle incoming client requests
}
