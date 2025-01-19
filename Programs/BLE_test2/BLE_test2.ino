#include <BluetoothSerial.h>

BluetoothSerial SerialBT;

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