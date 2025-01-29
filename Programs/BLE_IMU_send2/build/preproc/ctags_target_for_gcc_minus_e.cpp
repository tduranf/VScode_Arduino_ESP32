# 1 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\Classes_PROJ\\TP\\VScode_Arduino_ESP32\\Programs\\BLE_IMU_send2\\BLE_IMU_send2.ino"
# 2 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\Classes_PROJ\\TP\\VScode_Arduino_ESP32\\Programs\\BLE_IMU_send2\\BLE_IMU_send2.ino" 2
# 3 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\Classes_PROJ\\TP\\VScode_Arduino_ESP32\\Programs\\BLE_IMU_send2\\BLE_IMU_send2.ino" 2
# 4 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\Classes_PROJ\\TP\\VScode_Arduino_ESP32\\Programs\\BLE_IMU_send2\\BLE_IMU_send2.ino" 2

MPU9250 mpu;
BluetoothSerial SerialBT;

void setup() {
  Serial0.begin(115200);
  SerialBT.begin("ESP32_IMU_RPY");
  Wire.begin();
  delay(2000);

  if (!mpu.setup(0x68)) {
    while (1) {
      Serial0.println("MPU connection failed. Please check your connection with `connection_check` example.");
      delay(5000);
    }
  }
  Serial0.println("MPU connected");
  Serial0.println("Bluetooth started. Scan now!");
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
  Serial0.print("Roll, Pitch, Yaw: ");
  Serial0.print(roll, 0); Serial0.print(", ");
  Serial0.print(pitch, 0); Serial0.print(", ");
  Serial0.println(yaw, 0);

  // Send data via Bluetooth (CSV format)
  if (SerialBT.connected()) {
    SerialBT.println("Roll, Pitch, Yaw: ");
    SerialBT.print("Roll: "); SerialBT.print(roll, 0); SerialBT.print(",");
    SerialBT.print("Pitch: "); SerialBT.print(pitch, 0); SerialBT.print(",");
    SerialBT.print("Yaw: "); SerialBT.println(yaw, 0);
  } else {
    Serial0.println("Bluetooth not connected. Data only on Serial.");
  }
}
