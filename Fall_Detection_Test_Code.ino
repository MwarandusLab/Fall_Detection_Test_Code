#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

// Thresholds
const float ACC_THRESHOLD = 2.0; // Acceleration threshold for detecting fall
const float GYRO_THRESHOLD = 100.0; // Gyro threshold for detecting fall

void setup() {
  Serial.begin(9600);
  Wire.begin();
  mpu.initialize();

  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed");
    while (1);
  }

  Serial.println("MPU6050 connection successful");
}

void loop() {
  int16_t ax, ay, az;
  int16_t gx, gy, gz;

  // Read raw accelerometer and gyroscope data
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // Convert raw data to Gs and degrees/sec
  float axG = ax / 16384.0;
  float ayG = ay / 16384.0;
  float azG = az / 16384.0;

  float gxDPS = gx / 131.0;
  float gyDPS = gy / 131.0;
  float gzDPS = gz / 131.0;

  // Calculate the magnitude of acceleration
  float accMagnitude = sqrt(axG * axG + ayG * ayG + azG * azG);

  // Check if acceleration exceeds the threshold
  if (accMagnitude > ACC_THRESHOLD) {
    Serial.println("Possible fall detected (acceleration)");
  }

  // Check if gyroscope data exceeds the threshold
  if (abs(gxDPS) > GYRO_THRESHOLD || abs(gyDPS) > GYRO_THRESHOLD || abs(gzDPS) > GYRO_THRESHOLD) {
    Serial.println("Possible fall detected (gyroscope)");
  }

  // Print the sensor data
  Serial.print("Acc: ");
  Serial.print(axG); Serial.print(", ");
  Serial.print(ayG); Serial.print(", ");
  Serial.print(azG); Serial.print(" | ");
  Serial.print("Gyro: ");
  Serial.print(gxDPS); Serial.print(", ");
  Serial.print(gyDPS); Serial.print(", ");
  Serial.println(gzDPS);

  delay(200); // Adjust delay as necessary
}
