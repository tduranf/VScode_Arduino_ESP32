#include <BluetoothSerial.h>

BluetoothSerial SerialBT;

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