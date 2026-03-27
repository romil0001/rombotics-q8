// #include <Wire.h>
// #include <Adafruit_MPU6050.h>
// #include <Adafruit_PWMServoDriver.h> // Adafruit PCA9685 Library

// // Initialize components
// Adafruit_MPU6050 mpu;
// Adafruit_PWMServoDriver pca9685 = Adafruit_PWMServoDriver();

// void setup() {
//   Serial.begin(115200);

//   // Initialize MPU6050
//   if (!mpu.begin()) {
//     Serial.println("Failed to initialize MPU6050!");
//     while (1);
//   }

//   // Initialize PCA9685
//   pca9685.begin();
//   pca9685.setPWMFreq(60); // Set PWM frequency to 60 Hz
// }

// void loop() {
//   // Read MPU6050 sensor data
//   sensors_event_t accel, gyro, temp;
//   mpu.getEvent(&accel, &gyro, &temp);

//   // Calculate pitch and roll
//   float pitch = atan2(accel.acceleration.y, accel.acceleration.z) * 180 / M_PI;
//   float roll = atan2(-accel.acceleration.x, sqrt(accel.acceleration.y * accel.acceleration.y + accel.acceleration.z * accel.acceleration.z)) * 180 / M_PI;

//   // Example adjustments (replace with actual logic)
//   float shoulderAdjustments[4] = {pitch, -pitch, roll, -roll};
//   float kneeAngles[4] = {90.0, 90.0, 90.0, 90.0}; // Replace with real IK calculations

//   // Set servo positions (example for 4 servos)
//   for (int i = 0; i < 4; i++) {
//     int pulse = map(kneeAngles[i], 0, 180, 150, 600); // Map angle to pulse width
//     pca9685.setPWM(i, 0, pulse); // Set servo PWM
//   }

//   delay(100);
// }

#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_PWMServoDriver.h>

// Define constants for hardware
#define NUM_LEGS 4
#define SERVO_MIN 150 // Min pulse length for servos
#define SERVO_MAX 600 // Max pulse length for servos
#define STEP_PIN 2 // Example stepper pin
#define DIR_PIN 3  // Example stepper direction pin

Adafruit_MPU6050 mpu;
Adafruit_PWMServoDriver pca9685 = Adafruit_PWMServoDriver();

// Define servo and stepper positions
float kneeAngles[NUM_LEGS] = {90.0, 90.0, 90.0, 90.0};
int shoulderSteps[NUM_LEGS] = {0, 0, 0, 0};

void setup() {
  Serial.begin(115200);

  // Initialize MPU6050
  if (!mpu.begin()) {
    Serial.println("Failed to initialize MPU6050!");
    while (1);
  }

  // Initialize PCA9685
  pca9685.begin();
  pca9685.setPWMFreq(60); // 60 Hz frequency for servos

  // Initialize stepper motor pins
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
}

void loop() {
  // Read MPU6050 data
  sensors_event_t accel, gyro, temp;
  mpu.getEvent(&accel, &gyro, &temp);

  // Calculate pitch and roll
  float pitch = atan2(accel.acceleration.y, accel.acceleration.z) * 180 / M_PI;
  float roll = atan2(-accel.acceleration.x, sqrt(accel.acceleration.y * accel.acceleration.y + accel.acceleration.z * accel.acceleration.z)) * 180 / M_PI;

  // Compute adjustments for stabilization
  calculateStabilization(pitch, roll);

  // Apply adjustments to servos
  for (int i = 0; i < NUM_LEGS; i++) {
    int pulse = map(kneeAngles[i], 0, 180, SERVO_MIN, SERVO_MAX);
    pca9685.setPWM(i, 0, pulse);
  }

  // Apply adjustments to steppers (example for one leg)
  for (int i = 0; i < NUM_LEGS; i++) {
    moveStepper(shoulderSteps[i]); // Function to control steppers
  }

  delay(100); // Adjust as needed
}

void calculateStabilization(float pitch, float roll) {
  // Example: Adjust knee angles and shoulder steps based on pitch/roll
  kneeAngles[0] = 90.0 + pitch; // Adjust front-left knee
  kneeAngles[1] = 90.0 - pitch; // Adjust front-right knee
  kneeAngles[2] = 90.0 + roll;  // Adjust back-left knee
  kneeAngles[3] = 90.0 - roll;  // Adjust back-right knee

  // Adjust shoulder steps for steppers
  shoulderSteps[0] = pitch * 10; // Example scaling factor
  shoulderSteps[1] = -pitch * 10;
  shoulderSteps[2] = roll * 10;
  shoulderSteps[3] = -roll * 10;
}

void moveStepper(int steps) {
  // Example stepper control logic
  digitalWrite(DIR_PIN, steps > 0 ? HIGH : LOW);
  for (int i = 0; i < abs(steps); i++) {
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(500); // Adjust speed
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(500);
  }
}
