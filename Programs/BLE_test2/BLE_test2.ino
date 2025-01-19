#include <BluetoothSerial.h>

BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32_Test2");
  Serial.println("Bluetooth started. Scan now!"); // Clear message to scan now
}

void loop() {
  SerialBT.println("Hola2Mobil"); // Use println() to send the string with a newline
  Serial.println("Escric_SerialMonitor!");

  delay(1000);
}