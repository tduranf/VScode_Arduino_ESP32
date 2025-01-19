# 1 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\01_Eines_HW_SW\\03_SW\\11_Arduino_VScode\\ESP32_Tutorial\\VScode_Arduino_ESP32\\Programs\\BLE_test2\\BLE_test2.ino"
# 2 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\01_Eines_HW_SW\\03_SW\\11_Arduino_VScode\\ESP32_Tutorial\\VScode_Arduino_ESP32\\Programs\\BLE_test2\\BLE_test2.ino" 2

BluetoothSerial SerialBT;

void setup() {
  Serial0.begin(115200);
  SerialBT.begin("ESP32_Test2");
  Serial0.println("Bluetooth started. Scan now!"); // Clear message to scan now
}

void loop() {
  SerialBT.println("Hola2Mobil"); // Use println() to send the string with a newline
  Serial0.println("Escric_SerialMonitor!");

  delay(1000);
}
