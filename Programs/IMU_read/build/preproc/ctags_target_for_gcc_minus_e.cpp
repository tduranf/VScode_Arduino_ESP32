# 1 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\01_Eines_HW_SW\\03_SW\\11_Arduino_VScode\\ESP32_Tutorial\\VScode_Arduino_ESP32\\Programs\\IMU_read\\IMU_read.ino"
# 2 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\01_Eines_HW_SW\\03_SW\\11_Arduino_VScode\\ESP32_Tutorial\\VScode_Arduino_ESP32\\Programs\\IMU_read\\IMU_read.ino" 2
# 3 "C:\\Users\\puigm\\OneDrive - Universitat de Barcelona\\Documents\\02_Docencia_Manel\\01_Eines_HW_SW\\03_SW\\11_Arduino_VScode\\ESP32_Tutorial\\VScode_Arduino_ESP32\\Programs\\IMU_read\\IMU_read.ino" 2

IMU imu;
float *rpw;
const int PIN_IMU_INT = 18; // Interrupt pin

void setup() {
  Serial0.begin(115200);
  Wire.begin();
  pinMode(PIN_IMU_INT, 0x05);
  imu.Install();
}

void loop() {
  if (digitalRead(PIN_IMU_INT) == 0x1) { // Check for interrupt
    imu.ReadSensor();
    rpw = imu.GetRPW();
    if (rpw) { // Check if rpw is valid
        Serial0.print("Roll: "); Serial0.print(rpw[0]); Serial0.print("\t");
        Serial0.print("Pitch: "); Serial0.print(rpw[1]); Serial0.print("\t");
        Serial0.print("Yaw: "); Serial0.println(rpw[2]);
        }
        else {
        Serial0.println("Failed to get RPW values.");
        }
  }
  delay(50);
}
