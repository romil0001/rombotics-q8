#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// Create instances
Adafruit_MPU6050 mpu;
Adafruit_PWMServoDriver pca9685 = Adafruit_PWMServoDriver(0x40);

// Define servo parameters
#define SERVO_MIN 150 // Minimum pulse length out of 4096
#define SERVO_MAX 600 // Maximum pulse length out of 4096

void setup() {
  Serial.begin(115200);
  
  // Initialize MPU6050
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (true) delay(10);
  }
  
  // Configure MPU6050
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);

  // Initialize PCA9685
  pca9685.begin();
  pca9685.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
  
  delay(1000);
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Calculate servo positions based on sensor data
  int servoPositions[4];
  
  // Use roll angle from gyroscope for first two servos
  float roll = atan2(a.acceleration.y, a.acceleration.z) * RAD_TO_DEG;
  servoPositions[0] = map(constrain(roll, -45, 45), -45, 45, SERVO_MIN, SERVO_MAX);
  servoPositions[1] = map(constrain(roll, -45, 45), -45, 45, SERVO_MIN, SERVO_MAX);

  // Use pitch angle for second two servos
  float pitch = atan2(-a.acceleration.x, sqrt(a.acceleration.y*a.acceleration.y + a.acceleration.z*a.acceleration.z)) * RAD_TO_DEG;
  servoPositions[2] = map(constrain(pitch, -45, 45), -45, 45, SERVO_MIN, SERVO_MAX);
  servoPositions[3] = map(constrain(pitch, -45, 45), -45, 45, SERVO_MIN, SERVO_MAX);

  // Set servo positions
  for(int i = 0; i < 4; i++) {
    pca9685.setPWM(i, 0, servoPositions[i]);
  }

  delay(50);
}

// Helper function to convert angle to pulse width
uint16_t angleToPulse(int ang) {
  int pulse = map(ang, 0, 180, SERVO_MIN, SERVO_MAX);
  return constrain(pulse, SERVO_MIN, SERVO_MAX);
}