# 1 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\01_Eines_HW_SW\\03_SW\\11_Arduino_VScode\\ESP32_Tutorial\\VScode_Arduino_ESP32\\Programs\\IMU_send_BLE\\IMU_send_BLE.ino"
# 2 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\01_Eines_HW_SW\\03_SW\\11_Arduino_VScode\\ESP32_Tutorial\\VScode_Arduino_ESP32\\Programs\\IMU_send_BLE\\IMU_send_BLE.ino" 2
# 3 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\01_Eines_HW_SW\\03_SW\\11_Arduino_VScode\\ESP32_Tutorial\\VScode_Arduino_ESP32\\Programs\\IMU_send_BLE\\IMU_send_BLE.ino" 2
# 4 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\01_Eines_HW_SW\\03_SW\\11_Arduino_VScode\\ESP32_Tutorial\\VScode_Arduino_ESP32\\Programs\\IMU_send_BLE\\IMU_send_BLE.ino" 2

IMU imu;
float *rpw;

const int PIN_IMU_INT = 18;
//BluetoothSerial SerialBT;

void setup() {
  Serial0.begin(115200);
  //SerialBT.begin("ESP32_IMU"); // Bluetooth device name
  delay(500); // Add a small delay
  Serial0.println("The device started, now you can pair it with bluetooth!");
  Wire.begin();
  pinMode(PIN_IMU_INT, 0x05);
  imu.Install();
}

void loop() {
  if (digitalRead(PIN_IMU_INT) == 0x1) {
    imu.ReadSensor();
    rpw = imu.GetRPW();

    if (rpw) {
      // Send data via Serial Monitor
      //Serial.print("Roll: ");   Serial.print(rpw[0]); Serial.print("\t");
      //Serial.print("Pitch: ");  Serial.print(rpw[1]); Serial.print("\t");
      //Serial.print("Yaw: ");    Serial.println(rpw[2]);
      Serial0.println("BT OK!: ");
      // Send data via Bluetooth
    }
  }
}
