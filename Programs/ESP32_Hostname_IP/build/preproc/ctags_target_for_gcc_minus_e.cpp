# 1 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\01_Eines_HW_SW\\03_SW\\11_Arduino_VScode\\VScode_Arduino_ESP32\\Programs\\ESP32_Hostname_IP\\ESP32_Hostname_IP.ino"
# 2 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\01_Eines_HW_SW\\03_SW\\11_Arduino_VScode\\VScode_Arduino_ESP32\\Programs\\ESP32_Hostname_IP\\ESP32_Hostname_IP.ino" 2

// Credencials Wi-Fi
const char *ssid = "Robotics_UB";
const char *password = "rUBot_xx";

// Configuració de la IP fixa
IPAddress local_IP(192, 168, 0, 5);//IP desired
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(192, 168, 0, 1); // Opcional
IPAddress secondaryDNS(0, 0, 0, 0); // Opcional


void setup() {
  Serial0.begin(115200);
  delay(10);

  // Establir el Hostname
  WiFi.setHostname("ESP32_master_Group1");

  // Connectar-se a la xarxa Wi-Fi amb IP fixa
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial0.println("Configuració de la IP fallida.");
  }
  Serial0.println();
  Serial0.print("Connectant-se a ");
  Serial0.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial0.print(".");
  }

  Serial0.println();
  Serial0.println("WiFi connectat.");
  Serial0.print("Hostname: ");
  Serial0.println(WiFi.getHostname());
  Serial0.print("MAC: ");
  Serial0.println(WiFi.macAddress());
  Serial0.print("IP: ");
  Serial0.println(WiFi.localIP());
}

void loop() {
  Serial0.print("IP: ");
  Serial0.println(WiFi.localIP());
  delay(2000);
}
