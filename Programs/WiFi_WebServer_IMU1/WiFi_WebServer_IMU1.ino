#include <WiFi.h>
#include "src/RoboticsUB.h"

// Wi-Fi credentials
const char *ssid = "STRONG_ATRIA_AY4U_2.4";
const char *password = "3HCG3ykAUc";

// MPU-9250 object
IMU imu;

float *rpw;

const int PIN_IMU_INT = 18;

// Web server on port 80
WiFiServer server(80);

// Orientation data
float roll = 0.0, pitch = 0.0, yaw = 0.0;

// Function to handle JSON responses
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
  if (digitalRead(PIN_IMU_INT) == HIGH) {
    imu.ReadSensor();
    rpw = imu.GetRPW();
    if (rpw) {
      roll = rpw[0];
      pitch = rpw[1];
      yaw = rpw[2];
    } else {
      Serial.println("Failed to get RPW values.");
    }
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

  // Initialize the MPU-9250
  pinMode(PIN_IMU_INT, INPUT_PULLUP);
  imu.Install();

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