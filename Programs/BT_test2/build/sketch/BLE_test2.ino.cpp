#include <Arduino.h>
#line 1 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\Classes_PROJ\\TP\\VScode_Arduino_ESP32\\Programs\\BLE_test2\\BLE_test2.ino"
#include <Wire.h>
#include <BluetoothSerial.h>

BluetoothSerial SerialBT;

#line 6 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\Classes_PROJ\\TP\\VScode_Arduino_ESP32\\Programs\\BLE_test2\\BLE_test2.ino"
void setup();
#line 12 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\Classes_PROJ\\TP\\VScode_Arduino_ESP32\\Programs\\BLE_test2\\BLE_test2.ino"
void loop();
#line 6 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\Classes_PROJ\\TP\\VScode_Arduino_ESP32\\Programs\\BLE_test2\\BLE_test2.ino"
void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32_Test2");
  Serial.println("Bluetooth started. Scan now!"); // Clear message to scan now
}

void loop() {
  SerialBT.println("Writting_Mobile!"); // Use println() to send the string with a newline
  Serial.println("Writting_SerialMonitor!");

  delay(1000);
}
