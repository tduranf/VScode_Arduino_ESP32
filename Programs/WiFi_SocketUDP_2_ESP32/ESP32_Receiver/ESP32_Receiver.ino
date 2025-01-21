#include <WiFi.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>

// Add a device ID
const int deviceId = 2; // Has to be a number

// Wi-Fi credentials
const char *ssid = "STRONG_ATRIA_AY4U_2.4";
const char *password = "3HCG3ykAUc";

// UDP settings
const int udpPort = 12345;
WiFiUDP udp;

// Variables to store received data
float roll1 = 0.0, pitch1 = 0.0, yaw1 = 0.0;
float roll2 = 0.0, pitch2 = 0.0, yaw2 = 0.0;

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

void receiveOrientationUDP() {
  int packetSize = udp.parsePacket();
  if (packetSize) {
    byte packetBuffer[512];
    int len = udp.read(packetBuffer, 512);
    if (len > 0) {
      packetBuffer[len] = '\0';
      Serial.print("Received packet size: ");
      Serial.println(packetSize);
      Serial.print("Received: ");
      Serial.println((char*)packetBuffer);

      StaticJsonDocument<256> doc;
      DeserializationError error = deserializeJson(doc, packetBuffer);
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
      }

      const char* device = doc["device"];
      if (strcmp(device, "ESP32_1") == 0) {
        roll1 = doc["roll"];
        pitch1 = doc["pitch"];
        yaw1 = doc["yaw"];
        Serial.print("Roll_1: "); Serial.print(roll1); Serial.print(" Pitch_1: "); Serial.print(pitch1); Serial.print(" Yaw_1: "); Serial.println(yaw1);
      } else if (strcmp(device, "ESP32_2") == 0) {
        roll2 = doc["roll"];
        pitch2 = doc["pitch"];
        yaw2 = doc["yaw"];
        Serial.print("Roll_2: "); Serial.print(roll2); Serial.print(" Pitch_2: "); Serial.print(pitch2); Serial.print(" Yaw_2: "); Serial.println(yaw2);
      } else {
        Serial.println("Unknown device.");
      }
    }
  }
}

void setup() {
  Serial.begin(115200);
  connectToWiFi();
  udp.begin(udpPort);
  Serial.println("UDP receiver started");
}

void loop() {
  receiveOrientationUDP();
  delay(10);
}