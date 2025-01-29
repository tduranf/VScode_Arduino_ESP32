# 1 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\Classes_PROJ\\TP\\VScode_Arduino_ESP32\\Programs\\BLE_test1\\BLE_test1.ino"
# 2 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\Classes_PROJ\\TP\\VScode_Arduino_ESP32\\Programs\\BLE_test1\\BLE_test1.ino" 2
# 3 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\Classes_PROJ\\TP\\VScode_Arduino_ESP32\\Programs\\BLE_test1\\BLE_test1.ino" 2

BluetoothSerial SerialBT;

void setup() {
  Serial0.begin(115200);
  Wire.begin();
  SerialBT.begin("ESP32_Test1");
  Serial0.println("Bluetooth started. Scan now!"); // Clear message to scan now
}

void loop() {
  if (SerialBT.available()) {
    Serial0.write(SerialBT.read());
  }
  if (Serial0.available()) {
    SerialBT.write(Serial0.read());
  }
  //delay(1000);
}
