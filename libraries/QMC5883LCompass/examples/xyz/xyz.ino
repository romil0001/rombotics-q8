#include <Wire.h>  // Required for I2C communication
#include <MPU6050_tockn.h>  // MPU6050 library

MPU6050 mpu6050(Wire);

void setup() {
  Serial.begin(115200);
  
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);

  // Initialize motor control pins
  pinMode(34, OUTPUT);  // STEP pin
  pinMode(35, OUTPUT);  // DIR pin
}

void loop() {
  // Read accelerometer data
  mpu6050.update();
  
  // Calculate tilt angle (using X-axis for forward/backward tilt)
  float tiltAngle = atan2(mpu6050.getAccX(), sqrt(pow(mpu6050.getAccY(), 2) + pow(mpu6050.getAccZ(), 2))) * (180.0 / PI);
  
  // Determine motor direction based on tilt
  if (tiltAngle > 30) {  // Forward tilt
    digitalWrite(35, HIGH);  // Set direction to clockwise
  } else {  // Backward tilt
    digitalWrite(35, LOW);   // Set direction to counterclockwise
  }

  // Send step pulses
  digitalWrite(34, HIGH);
  delayMicroseconds(1000);
  digitalWrite(34, LOW);
  delayMicroseconds(1000);
}