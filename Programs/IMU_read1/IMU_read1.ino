#include <Wire.h>
#include "src/RoboticsUB.h"

IMU imu;
float *rpw;
const int PIN_IMU_INT = 18;       // Interrupt pin

void setup() {
  Serial.begin(115200);
  Wire.begin();
  pinMode(PIN_IMU_INT, INPUT_PULLUP);
  imu.Install();
}

void loop() {
  if (digitalRead(PIN_IMU_INT) == HIGH) { // Check for interrupt
    imu.ReadSensor();
    rpw = imu.GetRPW();
    if (rpw) { // Check if rpw is valid
        Serial.print("Roll: ");   Serial.print(rpw[0]); Serial.print("\t");
        Serial.print("Pitch: ");  Serial.print(rpw[1]); Serial.print("\t");
        Serial.print("Yaw: ");    Serial.println(rpw[2]);
        } 
        else {
        Serial.println("Failed to get RPW values.");
        }
  }
  delay(50);
}