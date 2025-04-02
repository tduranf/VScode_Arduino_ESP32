#include <Arduino.h>
#line 1 "/Users/tecla/Documents/Robotica/practiques/Projecte 2 Surgical Robotics/VScode_Arduino_ESP32/Programs/WiFi_SocketUDP_2_ESP32/ESP32_Servo_Reciever/ESP32_Servo_Reciever.ino"
#include <WiFi.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>
#include <ESP32Servo.h>

// Wi-Fi credentials
const char *ssid = "MIWIFI_2C34";
const char *password = "RSYZG2RC";

// UDP settings
const int udpPort = 12345;
WiFiUDP udp;

// Servos
Servo servo_yaw;
Servo servo_pitch;
Servo servo_roll1;
Servo servo_roll2;

// Pins
const int PIN_SIGNAL_YAW = 32;
const int PIN_SIGNAL_PITCH = 33;
const int PIN_SIGNAL_ROLL1 = 25;
const int PIN_SIGNAL_ROLL2 = 27;

// IMU data
float roll = 0.0, pitch = 0.0, yaw = 0.0;
float NewValueRoll = 0.0, OldValueRoll = 0.0;
float NewValuePitch = 0.0, OldValuePitch = 0.0;
float NewValueYaw = 0.0, OldValueYaw = 0.0;
int s1Status = HIGH;
int s2Status = HIGH;

#line 34 "/Users/tecla/Documents/Robotica/practiques/Projecte 2 Surgical Robotics/VScode_Arduino_ESP32/Programs/WiFi_SocketUDP_2_ESP32/ESP32_Servo_Reciever/ESP32_Servo_Reciever.ino"
void connectToWiFi();
#line 47 "/Users/tecla/Documents/Robotica/practiques/Projecte 2 Surgical Robotics/VScode_Arduino_ESP32/Programs/WiFi_SocketUDP_2_ESP32/ESP32_Servo_Reciever/ESP32_Servo_Reciever.ino"
void receiveOrientationUDP();
#line 141 "/Users/tecla/Documents/Robotica/practiques/Projecte 2 Surgical Robotics/VScode_Arduino_ESP32/Programs/WiFi_SocketUDP_2_ESP32/ESP32_Servo_Reciever/ESP32_Servo_Reciever.ino"
void setup();
#line 170 "/Users/tecla/Documents/Robotica/practiques/Projecte 2 Surgical Robotics/VScode_Arduino_ESP32/Programs/WiFi_SocketUDP_2_ESP32/ESP32_Servo_Reciever/ESP32_Servo_Reciever.ino"
void loop();
#line 34 "/Users/tecla/Documents/Robotica/practiques/Projecte 2 Surgical Robotics/VScode_Arduino_ESP32/Programs/WiFi_SocketUDP_2_ESP32/ESP32_Servo_Reciever/ESP32_Servo_Reciever.ino"
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

void receiveOrientationUDP() {
  int packetSize = udp.parsePacket();
  if (packetSize) {
    byte packetBuffer[512];
    int len = udp.read(packetBuffer, 512);
    if (len > 0) {
      packetBuffer[len] = '\0';
      StaticJsonDocument<256> doc;
      DeserializationError error = deserializeJson(doc, packetBuffer);
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
      }

      const char* device = doc["device"];
      if (strcmp(device, "G7_Gri") == 0) {
        Serial.print("Llegit de gripper: ");
        NewValueRoll = doc["roll"].as<float>();
        
        if (abs(NewValueRoll - OldValueRoll) > 10 && abs(NewValueRoll - OldValueRoll) < 350) {
          roll = OldValueRoll;
        } else {
          roll = NewValueRoll;
        }
        OldValueRoll = roll;

        NewValuePitch = doc["pitch"].as<float>();
        if (abs(NewValuePitch - OldValuePitch) > 10 && abs(NewValuePitch - OldValuePitch) < 350) {
          pitch = OldValuePitch;
        } else {
          pitch = NewValuePitch;
        }
        OldValuePitch = pitch;

        NewValueYaw = doc["yaw"].as<float>();
        if (abs(NewValueYaw - OldValueYaw) > 10 && abs(NewValueYaw - OldValueYaw) < 350) {
          yaw = OldValueYaw;
        } else {
          yaw = NewValueYaw;
        }
        OldValueYaw = yaw;
 
        s1Status = doc["s1Status"] | HIGH; 
        s2Status = doc["s2Status"] | HIGH;

        Serial.print("ROLL: "); Serial.print(roll);
        Serial.print(" | PITCH: "); Serial.print(pitch);
        Serial.print(" | YAW: "); Serial.println(yaw);
        Serial.print("s1Status: "); Serial.print(s1Status);
        Serial.print(" | s2Status: "); Serial.println(s2Status);

        // ➤ Control ROLL (amb 2 servos)
        if (roll > 180 && roll < 270) {
          servo_roll1.write(180);
          servo_roll2.write(0);
        } else if (roll > 270 && roll < 360) {
          servo_roll1.write(0);
          servo_roll2.write(180);
        } else if (!s1Status && !s2Status) {
          servo_roll1.write(roll);
          servo_roll2.write(180 - roll);
        }

        // ➤ Control PITCH
        float p = pitch;
        if (s1Status == LOW && s2Status == LOW) {
          if (p > 0 && p < 90) p += 90;
          else if (p > 270 && p < 360) p -= 270;
          servo_pitch.write(constrain(p, 0, 180));
        }

        // ➤ Control YAW
        float y = yaw;
        if (y > 180 && y <= 270) {
          servo_yaw.write(180);
        } else if (y > 270 && y <= 360) {
          servo_yaw.write(0);
        } else {
          if (s1Status == LOW && s2Status == LOW) {
            servo_yaw.write(constrain(y, 0, 180));
          }
        }
      } else {
        Serial.print("Device rebut: ");
        Serial.println(device);
      }
      
    }
  } else {
    Serial.println("No packet received");
  }
}

void setup() {
  Serial.begin(115200);
  connectToWiFi();
  udp.begin(udpPort);
  Serial.println("UDP receiver started");

  // Servo init
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);

  servo_yaw.setPeriodHertz(50);
  servo_pitch.setPeriodHertz(50);
  servo_roll1.setPeriodHertz(50);
  servo_roll2.setPeriodHertz(50);

  servo_yaw.attach(PIN_SIGNAL_YAW);
  servo_pitch.attach(PIN_SIGNAL_PITCH);
  servo_roll1.attach(PIN_SIGNAL_ROLL1);
  servo_roll2.attach(PIN_SIGNAL_ROLL2);

  servo_yaw.write(90);
  servo_pitch.write(90);
  servo_roll1.write(90);
  servo_roll2.write(90);
  delay(1000);
}

void loop() {
  receiveOrientationUDP();
  delay(10);
}

