# 1 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\01_Eines_HW_SW\\03_SW\\11_Arduino_VScode\\ESP32_Tutorial\\VScode_Arduino_ESP32\\Programs\\IMU_web_server\\IMU_web_server.ino"
# 2 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\01_Eines_HW_SW\\03_SW\\11_Arduino_VScode\\ESP32_Tutorial\\VScode_Arduino_ESP32\\Programs\\IMU_web_server\\IMU_web_server.ino" 2
# 3 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\01_Eines_HW_SW\\03_SW\\11_Arduino_VScode\\ESP32_Tutorial\\VScode_Arduino_ESP32\\Programs\\IMU_web_server\\IMU_web_server.ino" 2
# 4 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\01_Eines_HW_SW\\03_SW\\11_Arduino_VScode\\ESP32_Tutorial\\VScode_Arduino_ESP32\\Programs\\IMU_web_server\\IMU_web_server.ino" 2

// Wi-Fi credentials
const char* ssid = "STRONG_ATRIA_AY4U_2.4";
const char* password = "3HCG3ykAUc";

// MPU-9250 object
IMU imu;

const int PIN_IMU_INT = 18;

// Web server on port 80
WiFiServer server(80);

// Orientation data
float roll = 0.0, pitch = 0.0, yaw = 0.0;

// JSON document for data
StaticJsonDocument<200> doc; // Adjust size as needed

// Connect to Wi-Fi (improved error handling)
void connectToWiFi() {
  Serial0.print("Connecting to Wi-Fi");
  WiFi.begin(ssid, password);
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) { // Limit connection attempts
    delay(500); // Reduce delay for faster connection
    Serial0.print(".");
    attempts++;
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial0.println("\nWi-Fi connected!");
    Serial0.println("IP Address: " + WiFi.localIP().toString());
  } else {
    Serial0.println("\nWi-Fi connection failed!");
    while (true); // Halt if connection fails
  }
}

// Initialize the MPU-9250 (error handling improved)
void initIMU() {
  if (imu.begin() != 0) {
    Serial0.println("Unable to initialize the MPU-9250. Check connections.");
    while (true); // Halt program
  }
  Serial0.println("MPU-9250 initialized!");

  if (imu.dmpBegin((0x010) | (0x020), 10) != 0) {
    Serial0.println("Unable to enable DMP features.");
    while (true);
  }
  Serial0.println("DMP enabled.");
}

// Update orientation data
void updateOrientation() {
  if (digitalRead(PIN_IMU_INT) == 0x1) {
    imu.ReadSensor();
    float *rpw = imu.GetRPW(); // Directly use the pointer
    if (rpw) {
      roll = rpw[0];
      pitch = rpw[1];
      yaw = rpw[2];
    } else {
      Serial0.println("Failed to get RPW values.");
    }
  }
}

void handleWebRequest() {
  WiFiClient client = server.available();
  if (client) {
    String request = client.readStringUntil('\r');
    if (request.indexOf("GET /data") >= 0) {
      doc["roll"] = roll;
      doc["pitch"] = pitch;
      doc["yaw"] = yaw;
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: application/json");
      client.println();
      serializeJson(doc, client); // Efficient JSON serialization
    } else {
      // Serve a minimal HTML page (no need to rebuild it every time)
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println();
      client.println("<!DOCTYPE html><html><head><title>MPU9250</title><meta http-equiv='refresh' content='1'></head><body><h1>Orientation</h1><p>Roll: <span id='roll'></span>&deg;</p><p>Pitch: <span id='pitch'></span>&deg;</p><p>Yaw: <span id='yaw'></span>&deg;</p><script>fetch('/data').then(r=>r.json()).then(d=>{document.getElementById('roll').innerText=d.roll;document.getElementById('pitch').innerText=d.pitch;document.getElementById('yaw').innerText=d.yaw;});</script></body></html>");
    }
    client.stop();
  }
}

void setup() {
  Serial0.begin(115200);
  pinMode(PIN_IMU_INT, 0x05);
  connectToWiFi();
  initIMU();
  server.begin();
}

void loop() {
  updateOrientation();
  handleWebRequest();
  // No delay needed as handleWebRequest already takes time
}
