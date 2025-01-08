#include <WiFi.h>
#include <PubSubClient.h>
#include "src/RoboticsUB.h"

// Wi-Fi credentials
const char* ssid = "STRONG_ATRIA_AY4U_2.4";
const char* password = "3HCG3ykAUc";

// MQTT broker address
const char* mqtt_server = "broker.hivemq.com"; // Broker públic per a proves
const char* mqtt_topic = "esp32/imu/data";     // Tema per publicar dades

WiFiClient espClient;
PubSubClient client(espClient);

IMU imu;
float roll = 0.0, pitch = 0.0, yaw = 0.0;

const int PIN_IMU_INT = 18;

// Connect to Wi-Fi
void connectToWiFi() {
  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi connected!");
  Serial.println("IP Address: " + WiFi.localIP().toString());
}

// Reconnect to MQTT broker
void reconnectMQTT() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");
    if (client.connect("ESP32Client")) { // Nom del client MQTT
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" trying again in 5 seconds");
      delay(5000);
    }
  }
}

// Initialize the MPU-9250
void initIMU() {
  if (imu.begin() != INV_SUCCESS) {
    Serial.println("Unable to initialize the MPU-9250. Check connections.");
    while (true); // Halt program
  }
  Serial.println("MPU-9250 initialized!");

  // Enable DMP
  if (imu.dmpBegin(DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_GYRO_CAL, 10) != INV_SUCCESS) {
    Serial.println("Unable to enable DMP features.");
    while (true); // Halt program
  }
  Serial.println("DMP enabled.");
}

// Interrupt handler for IMU
void IRAM_ATTR handleIMUInterrupt() {
  imu.ReadSensor();
  float* rpw = imu.GetRPW();
  if (rpw) {
    roll = rpw[0];
    pitch = rpw[1];
    yaw = rpw[2];
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(PIN_IMU_INT, INPUT_PULLUP);

  // Connect to Wi-Fi
  Serial.println("Starting ESP32...");
  connectToWiFi();

  // Initialize MQTT client
  client.setServer(mqtt_server, 1883);

  // Initialize the MPU-9250
  initIMU();

  // Attach interrupt for IMU data
  attachInterrupt(digitalPinToInterrupt(PIN_IMU_INT), handleIMUInterrupt, RISING);

  Serial.println("Setup complete.");
}

void loop() {
  if (!client.connected()) {
    reconnectMQTT();
  }
  client.loop();

  // Publish IMU data as JSON
  String json = "{\"roll\":" + String(roll, 2) + ",\"pitch\":" + String(pitch, 2) + ",\"yaw\":" + String(yaw, 2) + "}";
  client.publish(mqtt_topic, json.c_str());

  delay(10); // Evita saturar el broker, però manté baixa latència
}
