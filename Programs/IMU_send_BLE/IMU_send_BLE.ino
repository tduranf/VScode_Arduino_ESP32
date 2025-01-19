#include <Wire.h>
#include "src/RoboticsUB.h"
#include <BluetoothSerial.h>

IMU imu;
float *rpw;

const int PIN_IMU_INT = 18;
//BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  //SerialBT.begin("ESP32_IMU"); // Bluetooth device name
  delay(500); // Add a small delay
  Serial.println("The device started, now you can pair it with bluetooth!");
  Wire.begin();
  pinMode(PIN_IMU_INT, INPUT_PULLUP);
  imu.Install();
}

void loop() {
  if (digitalRead(PIN_IMU_INT) == HIGH) {
    imu.ReadSensor();
    rpw = imu.GetRPW();

    if (rpw) {
      // Send data via Serial Monitor
      //Serial.print("Roll: ");   Serial.print(rpw[0]); Serial.print("\t");
      //Serial.print("Pitch: ");  Serial.print(rpw[1]); Serial.print("\t");
      //Serial.print("Yaw: ");    Serial.println(rpw[2]);
      Serial.println("BT OK!: ");
      // Send data via Bluetooth
    }
  }
}