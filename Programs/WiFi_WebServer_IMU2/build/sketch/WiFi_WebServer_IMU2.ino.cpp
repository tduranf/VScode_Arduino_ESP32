#include <Arduino.h>
#line 1 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\Classes_PROJ\\TP\\VScode_Arduino_ESP32\\Programs\\WiFi_WebServer_IMU2\\WiFi_WebServer_IMU2.ino"
#include <WiFi.h>
#include <Wire.h>
#include "MPU9250.h"


// Wi-Fi credentials
const char *ssid = "STRONG_ATRIA_AY4U_2.4";
const char *password = "3HCG3ykAUc";

// MPU-9250 object
MPU9250 mpu;

float *rpw;

// Web server on port 80
WiFiServer server(80);

// Orientation data
float roll = 0.0, pitch = 0.0, yaw = 0.0;

// Function to handle JSON responses
#line 22 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\Classes_PROJ\\TP\\VScode_Arduino_ESP32\\Programs\\WiFi_WebServer_IMU2\\WiFi_WebServer_IMU2.ino"
void sendJsonResponse(WiFiClient client);
#line 37 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\Classes_PROJ\\TP\\VScode_Arduino_ESP32\\Programs\\WiFi_WebServer_IMU2\\WiFi_WebServer_IMU2.ino"
void sendHtmlResponse(WiFiClient client);
#line 63 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\Classes_PROJ\\TP\\VScode_Arduino_ESP32\\Programs\\WiFi_WebServer_IMU2\\WiFi_WebServer_IMU2.ino"
void connectToWiFi();
#line 75 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\Classes_PROJ\\TP\\VScode_Arduino_ESP32\\Programs\\WiFi_WebServer_IMU2\\WiFi_WebServer_IMU2.ino"
void updateOrientation();
#line 84 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\Classes_PROJ\\TP\\VScode_Arduino_ESP32\\Programs\\WiFi_WebServer_IMU2\\WiFi_WebServer_IMU2.ino"
void handleWebRequest();
#line 97 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\Classes_PROJ\\TP\\VScode_Arduino_ESP32\\Programs\\WiFi_WebServer_IMU2\\WiFi_WebServer_IMU2.ino"
void setup();
#line 118 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\Classes_PROJ\\TP\\VScode_Arduino_ESP32\\Programs\\WiFi_WebServer_IMU2\\WiFi_WebServer_IMU2.ino"
void loop();
#line 22 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\Classes_PROJ\\TP\\VScode_Arduino_ESP32\\Programs\\WiFi_WebServer_IMU2\\WiFi_WebServer_IMU2.ino"
void sendJsonResponse(WiFiClient client) {
  String json = "{";
  json += "\"roll\":" + String(roll, 2) + ",";
  json += "\"pitch\":" + String(pitch, 2) + ",";
  json += "\"yaw\":" + String(yaw, 2);
  json += "}";

  client.println("HTTP/1.1 200 OK");
  client.println("Content-type: application/json"); // Correct Content-Type
  client.println("Connection: close"); // Important for ESP32
  client.println();
  client.println(json);
}

// Function to handle HTML responses
void sendHtmlResponse(WiFiClient client) {
  String webpage = "<!DOCTYPE html><html><head><title>MPU9250 Orientation</title>";
  webpage += "<meta name='viewport' content='width=device-width, initial-scale=1'>"; // For mobile responsiveness
  webpage += "<script>";
  webpage += "setInterval(() => { fetch('/data').then(res => res.json()).then(data => {";
  webpage += "document.getElementById('roll').innerText = data.roll;";
  webpage += "document.getElementById('pitch').innerText = data.pitch;";
  webpage += "document.getElementById('yaw').innerText = data.yaw;";
  webpage += "}); }, 100);"; // Reduced interval for faster updates (adjust as needed)
  webpage += "</script>";
  webpage += "</head><body>";
  webpage += "<h1>MPU-9250 Orientation</h1>";
  webpage += "<p>Roll: <span id='roll'>0</span> &deg;</p>";
  webpage += "<p>Pitch: <span id='pitch'>0</span> &deg;</p>";
  webpage += "<p>Yaw: <span id='yaw'>0</span> &deg;</p>";
  webpage += "</body></html>";

  client.println("HTTP/1.1 200 OK");
  client.println("Content-type: text/html");
  client.println("Connection: close"); // Important for ESP32
  client.println();
  client.println(webpage);
}


// Connect to Wi-Fi (unchanged)
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

// Update orientation data (unchanged)
void updateOrientation() {
  if (mpu.update()) {
    yaw = -mpu.getYaw();
    pitch = -mpu.getPitch();
    roll = mpu.getRoll();
    }
}

// Handle incoming client requests
void handleWebRequest() {
  WiFiClient client = server.available();
  if (client) {
    String request = client.readStringUntil('\r');
    if (request.indexOf("GET /data") >= 0) {
      sendJsonResponse(client);
    } else {
      sendHtmlResponse(client);
    }
    client.stop(); // Very important to close the connection
  }
}

void setup() {
  Serial.begin(115200);
  Wire.begin();
  delay(2000);

  // Initialize the MPU-9250
  if (!mpu.setup(0x68)) {
    while (1) {
      Serial.println("MPU connection failed. Please check your connection with `connection_check` example.");
      delay(5000);
    }
  }
  Serial.println("MPU connected");
  delay(2000);

  // Connect to Wi-Fi
  Serial.println("Starting ESP32...");
  connectToWiFi();
  server.begin();
}

void loop() {
  updateOrientation();
  handleWebRequest();
  delay(10); // Adjust as needed
}
