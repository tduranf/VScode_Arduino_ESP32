#include <Arduino.h>
#line 1 "/Users/tecla/Documents/Pràctiques EBM/Robòtica/practiques/projecte 2 surgical robotics/esp32/VScode_Arduino_ESP32/Programs/IMU_read2/IMU_read2.ino"
#include <Wire.h>
#include "MPU9250.h"

MPU9250 mpu;

#line 6 "/Users/tecla/Documents/Pràctiques EBM/Robòtica/practiques/projecte 2 surgical robotics/esp32/VScode_Arduino_ESP32/Programs/IMU_read2/IMU_read2.ino"
void setup();
#line 20 "/Users/tecla/Documents/Pràctiques EBM/Robòtica/practiques/projecte 2 surgical robotics/esp32/VScode_Arduino_ESP32/Programs/IMU_read2/IMU_read2.ino"
void loop();
#line 30 "/Users/tecla/Documents/Pràctiques EBM/Robòtica/practiques/projecte 2 surgical robotics/esp32/VScode_Arduino_ESP32/Programs/IMU_read2/IMU_read2.ino"
void send_rpy_serial();
#line 6 "/Users/tecla/Documents/Pràctiques EBM/Robòtica/practiques/projecte 2 surgical robotics/esp32/VScode_Arduino_ESP32/Programs/IMU_read2/IMU_read2.ino"
void setup() {
  Serial.begin(115200);
  Wire.begin();
  delay(2000);

  if (!mpu.setup(0x68)) {
    while (1) {
      Serial.println("MPU connection failed. Please check your connection with `connection_check` example.");
      delay(5000);
    }
  }
  Serial.println("MPU connected");
}

void loop() {
  if (mpu.update()) {
    static uint32_t prev_ms = millis();
    if (millis() > prev_ms + 200) { // Update and send data every 25ms (40Hz)
      prev_ms = millis();
      send_rpy_serial();
    }
  }
}

void send_rpy_serial() {
  float yaw = -mpu.getYaw();
  float pitch = -mpu.getPitch();
  float roll = mpu.getRoll();

  // Send data via Serial Monitor
  Serial.print("Roll, Pitch, Yaw: ");
  Serial.print(roll, 0); Serial.print(", ");
  Serial.print(pitch, 0); Serial.print(", ");
  Serial.println(yaw, 0);
}
