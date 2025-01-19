# 1 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\01_Eines_HW_SW\\03_SW\\11_Arduino_VScode\\ESP32_Tutorial\\VScode_Arduino_ESP32\\Programs\\blink\\blink.ino"
//Test program
const uint8_t led = 2;

void setup()
{
    Serial0.begin(9600);
 pinMode(led, 0x03);
}

void loop()
{
 digitalWrite(led, 0x1);
    Serial0.println("Led ences");
    delay(2000);
    digitalWrite(led, 0x0);
    Serial0.println("Led apagat");
    delay(2000);
}
