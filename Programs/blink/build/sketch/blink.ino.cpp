#include <Arduino.h>
#line 1 "/Users/tecla/Documents/Pràctiques EBM/Robòtica/practiques/projecte 2 surgical robotics/esp32/VScode_Arduino_ESP32/Programs/blink/blink.ino"
//Test program
const uint8_t led = 2;

#line 4 "/Users/tecla/Documents/Pràctiques EBM/Robòtica/practiques/projecte 2 surgical robotics/esp32/VScode_Arduino_ESP32/Programs/blink/blink.ino"
void setup();
#line 10 "/Users/tecla/Documents/Pràctiques EBM/Robòtica/practiques/projecte 2 surgical robotics/esp32/VScode_Arduino_ESP32/Programs/blink/blink.ino"
void loop();
#line 4 "/Users/tecla/Documents/Pràctiques EBM/Robòtica/practiques/projecte 2 surgical robotics/esp32/VScode_Arduino_ESP32/Programs/blink/blink.ino"
void setup()
{
    Serial.begin(9600);
	pinMode(led, OUTPUT);
}

void loop()
{
	digitalWrite(led, HIGH);
    Serial.println("Led ences");
    delay(2000);
    digitalWrite(led, LOW);
    Serial.println("Led apagat");
    delay(2000);
}

