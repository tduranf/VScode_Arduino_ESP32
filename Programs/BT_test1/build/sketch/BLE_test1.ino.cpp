#include <Arduino.h>
#line 1 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\Classes_PROJ\\TP\\VScode_Arduino_ESP32\\Programs\\BLE_test1\\BLE_test1.ino"
#include <Wire.h>
#include <BluetoothSerial.h>

BluetoothSerial SerialBT;

#line 6 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\Classes_PROJ\\TP\\VScode_Arduino_ESP32\\Programs\\BLE_test1\\BLE_test1.ino"
void setup();
#line 13 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\Classes_PROJ\\TP\\VScode_Arduino_ESP32\\Programs\\BLE_test1\\BLE_test1.ino"
void loop();
#line 6 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\Classes_PROJ\\TP\\VScode_Arduino_ESP32\\Programs\\BLE_test1\\BLE_test1.ino"
void setup() {
  Serial.begin(115200);
  Wire.begin();
  SerialBT.begin("ESP32_Test1");
  Serial.println("Bluetooth started. Scan now!"); // Clear message to scan now
}

void loop() {
  if (SerialBT.available()) {
    Serial.write(SerialBT.read());
  }
  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }
  //delay(1000);
}
