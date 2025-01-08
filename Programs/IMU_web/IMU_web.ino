#include <WiFi.h>
#include "src/RoboticsUB.h"

//#include <BluetoothSerial.h> // Add Bluetooth support

// Wi-Fi credentials
const char* ssid = "STRONG_ATRIA_AY4U_2.4";
const char* password = "3HCG3ykAUc";

// MPU-9250 object
IMU imu;

enum class Command : byte
{
  GET_RPW = 1
};

Command command = Command::GET_RPW;
float *rpw;

const int PIN_IMU_INT = 18;
const int PIN_S1 = 14;
const int PIN_S2 = 27;

// Web server on port 80
WiFiServer server(80);
//BluetoothSerial SerialBT; // Bluetooth Serial object

// Orientation data
float roll = 0.0, pitch = 0.0, yaw = 0.0;

// HTML page content
String webpage = "";

// Connect to Wi-Fi
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

// Initialize the MPU-9250
void initIMU() {
  if (imu.begin() != INV_SUCCESS) {
    Serial.println("Unable to initialize the MPU-9250. Check connections.");
    while (true); // Halt program
  }
  Serial.println("MPU-9250 initialized!");

  // Enable DMP
  if (imu.dmpBegin(DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_GYRO_CAL, 10) != INV_SUCCESS) {
    Serial.println("Unable to enable DMP features.");
    while (true); // Halt program
  }
  Serial.println("DMP enabled.");
}

// Update orientation data
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

// Generate the HTML page
void updateWebpage() {
    webpage = "<!DOCTYPE html><html><head><title>MPU9250 Orientation</title>";
    webpage += "<script>";
    webpage += "setInterval(() => { fetch('/data').then(res => res.json()).then(data => {";
    webpage += "document.getElementById('roll').innerText = data.roll;";
    webpage += "document.getElementById('pitch').innerText = data.pitch;";
    webpage += "document.getElementById('yaw').innerText = data.yaw;";
    webpage += "}); }, 1000);"; // Fetch data every second
    webpage += "</script>";
    webpage += "</head><body>";
    webpage += "<h1>MPU-9250 Orientation</h1>";
    webpage += "<p>Roll: <span id='roll'>0</span> &deg;</p>";
    webpage += "<p>Pitch: <span id='pitch'>0</span> &deg;</p>";
    webpage += "<p>Yaw: <span id='yaw'>0</span> &deg;</p>";
    webpage += "</body></html>";
}

// Handle incoming client requests
void handleWebRequest() {
    WiFiClient client = server.available();
    if (client) {
        String request = client.readStringUntil('\r');
        if (request.indexOf("GET /data") >= 0) {
            // Respond with JSON data
            String json = "{";
            json += "\"roll\":" + String(roll, 2) + ",";
            json += "\"pitch\":" + String(pitch, 2) + ",";
            json += "\"yaw\":" + String(yaw, 2);
            json += "}";
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:application/json");
            client.println();
            client.println(json);
        } else {
            // Respond with the webpage
            updateWebpage();
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            client.println(webpage);
        }
        client.stop();
    }
}

void setup() {
  Serial.begin(115200);

  pinMode(PIN_IMU_INT, INPUT_PULLUP);

  // Connect to Wi-Fi
  Serial.println("Starting ESP32...");
  connectToWiFi();
  server.begin();

  // Initialize Bluetooth
  //SerialBT.begin("ESP32_MPU9250"); // Set Bluetooth device name
  //Serial.println("Bluetooth initialized. Waiting for connections...");

  // Initialize the MPU-9250
  initIMU();
}

void loop() {
  updateOrientation(); // Get the latest RPY values
  handleWebRequest();  // Handle client requests

  // Send RPY values over Bluetooth
  //if (SerialBT.connected()) {
  //  SerialBT.printf("Roll: %.2f, Pitch: %.2f, Yaw: %.2f\n", roll, pitch, yaw);
  //}

  delay(20); // Update every 100 ms
}
