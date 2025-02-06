#include <WiFi.h>

// Credencials Wi-Fi
const char *ssid = "Robotics_UB";
const char *password = "rUBot_xx";

// Configuració de la IP fixa
IPAddress local_IP(192, 168, 0, 5);//IP desired
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(192, 168, 0, 1);   // Opcional
IPAddress secondaryDNS(0, 0, 0, 0); // Opcional


void setup() {
  Serial.begin(115200);
  delay(10);

  // Establir el Hostname
  WiFi.setHostname("ESP32_master_Group1");

  // Connectar-se a la xarxa Wi-Fi amb IP fixa
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("Configuració de la IP fallida.");
  }
  Serial.println();
  Serial.print("Connectant-se a ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connectat.");
  Serial.print("Hostname: ");
  Serial.println(WiFi.getHostname());
  Serial.print("MAC: ");
  Serial.println(WiFi.macAddress());
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  delay(2000);
}
