# 1 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\Classes_PROJ\\TP\\VScode_Arduino_ESP32\\Programs\\BLE_test2\\BLE_test2.ino"
# 2 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\Classes_PROJ\\TP\\VScode_Arduino_ESP32\\Programs\\BLE_test2\\BLE_test2.ino" 2
# 3 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\Classes_PROJ\\TP\\VScode_Arduino_ESP32\\Programs\\BLE_test2\\BLE_test2.ino" 2

BluetoothSerial SerialBT;

void setup() {
  Serial0.begin(115200);
  SerialBT.begin("ESP32_Test2");
  Serial0.println("Bluetooth started. Scan now!"); // Clear message to scan now
}

void loop() {
  SerialBT.println("Writting_Mobile!"); // Use println() to send the string with a newline
  Serial0.println("Writting_SerialMonitor!");

  delay(1000);
}
