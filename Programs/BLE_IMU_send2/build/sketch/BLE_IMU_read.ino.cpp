#include <Arduino.h>
#line 1 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\01_Eines_HW_SW\\03_SW\\11_Arduino_VScode\\ESP32_Tutorial\\VScode_Arduino_ESP32\\Programs\\BLE_IMU_read\\BLE_IMU_read.ino"
#include <Wire.h>
#include "MPU9250.h"
#include <BluetoothSerial.h>

MPU9250 mpu;
BluetoothSerial SerialBT;

#line 8 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\01_Eines_HW_SW\\03_SW\\11_Arduino_VScode\\ESP32_Tutorial\\VScode_Arduino_ESP32\\Programs\\BLE_IMU_read\\BLE_IMU_read.ino"
void setup();
#line 24 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\01_Eines_HW_SW\\03_SW\\11_Arduino_VScode\\ESP32_Tutorial\\VScode_Arduino_ESP32\\Programs\\BLE_IMU_read\\BLE_IMU_read.ino"
void loop();
#line 34 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\01_Eines_HW_SW\\03_SW\\11_Arduino_VScode\\ESP32_Tutorial\\VScode_Arduino_ESP32\\Programs\\BLE_IMU_read\\BLE_IMU_read.ino"
void send_rpy_bluetooth();
#line 8 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\01_Eines_HW_SW\\03_SW\\11_Arduino_VScode\\ESP32_Tutorial\\VScode_Arduino_ESP32\\Programs\\BLE_IMU_read\\BLE_IMU_read.ino"
void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32_IMU_RPY");
  Wire.begin();
  delay(2000);

  if (!mpu.setup(0x68)) {
    while (1) {
      Serial.println("MPU connection failed. Please check your connection with `connection_check` example.");
      delay(5000);
    }
  }
  Serial.println("MPU connected");
  Serial.println("Bluetooth started. Scan now!");
}

void loop() {
  if (mpu.update()) {
    static uint32_t prev_ms = millis();
    if (millis() > prev_ms + 200) { // Update and send data every 25ms (40Hz)
      prev_ms = millis();
      send_rpy_bluetooth();
    }
  }
}

void send_rpy_bluetooth() {
  float yaw = -mpu.getYaw();
  float pitch = -mpu.getPitch();
  float roll = mpu.getRoll();

  // Send data via Serial Monitor
  Serial.print("Roll, Pitch, Yaw: ");
  Serial.print(roll, 0); Serial.print(", ");
  Serial.print(pitch, 0); Serial.print(", ");
  Serial.println(yaw, 0);

  // Send data via Bluetooth (CSV format)
  if (SerialBT.connected()) {
    SerialBT.print(roll, 0); SerialBT.print(",");
    SerialBT.print(pitch, 0); SerialBT.print(",");
    SerialBT.println(yaw, 0);
  } else {
    Serial.println("Bluetooth not connected. Data only on Serial.");
  }
}
