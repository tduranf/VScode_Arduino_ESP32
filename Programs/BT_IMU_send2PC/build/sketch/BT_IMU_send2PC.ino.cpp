#include <Arduino.h>
#line 1 "C:\\Users\\manel.puig\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\Classes_PROJ\\TP\\VScode_Arduino_ESP32\\Programs\\BT_IMU_send2PC\\BT_IMU_send2PC.ino"
#include <Wire.h>
#include "MPU9250.h"
#include <BluetoothSerial.h>

MPU9250 mpu;
BluetoothSerial SerialBT;

#line 8 "C:\\Users\\manel.puig\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\Classes_PROJ\\TP\\VScode_Arduino_ESP32\\Programs\\BT_IMU_send2PC\\BT_IMU_send2PC.ino"
void setup();
#line 24 "C:\\Users\\manel.puig\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\Classes_PROJ\\TP\\VScode_Arduino_ESP32\\Programs\\BT_IMU_send2PC\\BT_IMU_send2PC.ino"
void loop();
#line 34 "C:\\Users\\manel.puig\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\Classes_PROJ\\TP\\VScode_Arduino_ESP32\\Programs\\BT_IMU_send2PC\\BT_IMU_send2PC.ino"
void send_rpy_bluetooth();
#line 8 "C:\\Users\\manel.puig\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\Classes_PROJ\\TP\\VScode_Arduino_ESP32\\Programs\\BT_IMU_send2PC\\BT_IMU_send2PC.ino"
void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32_IMU_RPY"); // Nom del dispositiu Bluetooth
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
    if (millis() > prev_ms + 25) { // Envia dades cada 25ms (40Hz)
      prev_ms = millis();
      send_rpy_bluetooth();
    }
  }
}

void send_rpy_bluetooth() {
  float yaw = -mpu.getYaw();
  float pitch = -mpu.getPitch();
  float roll = mpu.getRoll();

  // Envia dades al monitor sèrie (per depuració)
  Serial.print("Roll, Pitch, Yaw: ");
  Serial.print(roll, 1); Serial.print(", ");
  Serial.print(pitch, 1); Serial.print(", ");
  Serial.println(yaw, 1);

  // Envia dades per Bluetooth (format CSV, una sola línia)
  if (SerialBT.connected()) {
    SerialBT.print(roll, 1); SerialBT.print(",");
    SerialBT.print(pitch, 1); SerialBT.print(",");
    SerialBT.println(yaw, 1);
  } else {
    Serial.println("Bluetooth not connected. Data only on Serial.");
  }
}
