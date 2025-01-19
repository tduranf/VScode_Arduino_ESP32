#include <Arduino.h>
#line 1 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\01_Eines_HW_SW\\03_SW\\11_Arduino_VScode\\ESP32_Tutorial\\VScode_Arduino_ESP32\\Programs\\BLE_test\\BLE_test.ino"
#include <Wire.h>
//#include "src/RoboticsUB.h"
#include <BluetoothSerial.h>

BluetoothSerial SerialBT;

#line 7 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\01_Eines_HW_SW\\03_SW\\11_Arduino_VScode\\ESP32_Tutorial\\VScode_Arduino_ESP32\\Programs\\BLE_test\\BLE_test.ino"
void setup();
#line 14 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\01_Eines_HW_SW\\03_SW\\11_Arduino_VScode\\ESP32_Tutorial\\VScode_Arduino_ESP32\\Programs\\BLE_test\\BLE_test.ino"
void loop();
#line 7 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\01_Eines_HW_SW\\03_SW\\11_Arduino_VScode\\ESP32_Tutorial\\VScode_Arduino_ESP32\\Programs\\BLE_test\\BLE_test.ino"
void setup() {
  Serial.begin(115200);
  Wire.begin();
  SerialBT.begin("ESP32_Test2");
  Serial.println("Bluetooth started. Scan now!"); // Clear message to scan now
}

void loop() {
  if (SerialBT.available()) {
    Serial.write(SerialBT.read());
  }
  if (Serial.available()) {
    SerialBT.write(Serial.read());
    SerialBT.println("Hola2Mobil"); // Use println() to send the string with a newline
    Serial.println("Escric_SerialMonitor!");
  }
  delay(1000);
}
