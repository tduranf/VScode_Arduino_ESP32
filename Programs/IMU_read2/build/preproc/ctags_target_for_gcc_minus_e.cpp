# 1 "/Users/tecla/Documents/Pràctiques EBM/Robòtica/practiques/projecte 2 surgical robotics/esp32/VScode_Arduino_ESP32/Programs/IMU_read2/IMU_read2.ino"
# 2 "/Users/tecla/Documents/Pràctiques EBM/Robòtica/practiques/projecte 2 surgical robotics/esp32/VScode_Arduino_ESP32/Programs/IMU_read2/IMU_read2.ino" 2
# 3 "/Users/tecla/Documents/Pràctiques EBM/Robòtica/practiques/projecte 2 surgical robotics/esp32/VScode_Arduino_ESP32/Programs/IMU_read2/IMU_read2.ino" 2

MPU9250 mpu;

void setup() {
  Serial0.begin(115200);
  Wire.begin();
  delay(2000);

  if (!mpu.setup(0x68)) {
    while (1) {
      Serial0.println("MPU connection failed. Please check your connection with `connection_check` example.");
      delay(5000);
    }
  }
  Serial0.println("MPU connected");
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
  Serial0.print("Roll, Pitch, Yaw: ");
  Serial0.print(roll, 0); Serial0.print(", ");
  Serial0.print(pitch, 0); Serial0.print(", ");
  Serial0.println(yaw, 0);
}
