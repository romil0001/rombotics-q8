#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_PWMServoDriver.h>
#include <AccelStepper.h>

// Define constants
#define NUM_LEGS 4
#define SERVO_MIN 150
#define SERVO_MAX 600
#define STEPS_PER_REV 200 // Steps per revolution for NEMA 17
#define STEP_PINS {2, 4, 6, 8} // Step pins for 4 stepper motors
#define DIR_PINS {3, 5, 7, 9}  // Direction pins for 4 stepper motors

// PID parameters
float Kp = 10.0f;
float Ki = 0.1f;
float Kd = 5.0f;

// State variables
float integral = 0.0f;

// MPU6050 and PCA9685
Adafruit_MPU6050 mpu;
Adafruit_PWMServoDriver pca9685 = Adafruit_PWMServoDriver();

// AccelStepper objects for 4 stepper motors
AccelStepper steppers[NUM_LEGS] = {
  AccelStepper(AccelStepper::DRIVER, 2, 3),
  AccelStepper(AccelStepper::DRIVER, 4, 5),
  AccelStepper(AccelStepper::DRIVER, 6, 7),
  AccelStepper(AccelStepper::DRIVER, 8, 9)
};

// Leg states
float knee_angles[NUM_LEGS] = {90.0, 90.0, 90.0, 90.0};
float shoulder_positions[NUM_LEGS] = {0, 0, 0, 0};

void setup() {
  Serial.begin(115200);

  // Initialize MPU6050
  if (!mpu.begin()) {
    Serial.println("Failed to initialize MPU6050!");
    while (1);
  }

  // Initialize PCA9685
  pca9685.begin();
  pca9685.setPWMFreq(60);

  // Initialize steppers
  for (int i = 0; i < NUM_LEGS; i++) {
    steppers[i].setMaxSpeed(1000);  // Maximum speed for stepper motors
    steppers[i].setAcceleration(500); // Acceleration for smooth motion
  }
}

void loop() {
  // Read MPU6050 sensor data
  sensors_event_t accel, gyro, temp;
  mpu.getEvent(&accel, &gyro, &temp);

  // Calculate pitch and roll
  float pitch = atan2(accel.acceleration.y, accel.acceleration.z) * 180 / M_PI;
  float roll = atan2(-accel.acceleration.x, 
                     sqrt(accel.acceleration.y * accel.acceleration.y + accel.acceleration.z * accel.acceleration.z)) * 180 / M_PI;

  // PID adjustments for pitch and roll
  float pid_pitch = pidController(pitch);
  float pid_roll = pidController(roll);

  // Update leg states
  updateLegStates(pid_pitch, pid_roll);

  // Apply servo and stepper adjustments
  applyServoAdjustments();
  applyStepperAdjustments();

  delay(20); // Small delay for stability
}

float pidController(float angle) {
  static float lastError = 0;
  float error = angle; // Error based on the target being 0
  integral += error * Ki;
  float derivative = (error - lastError) * Kd;
  lastError = error;

  return Kp * error + integral + derivative;
}

void updateLegStates(float pid_pitch, float pid_roll) {
  // Calculate adjustments for each leg
  float leg_adjustment[NUM_LEGS];
  for (int i = 0; i < NUM_LEGS; i++) {
    leg_adjustment[i] = pid_pitch * cos(i * PI / 2) + pid_roll * sin(i * PI / 2);
  }

  // Update knee angles and shoulder positions
  for (int i = 0; i < NUM_LEGS; i++) {
    knee_angles[i] = constrain(90.0 + leg_adjustment[i], 0, 180); // Constrain knee angles
    shoulder_positions[i] = leg_adjustment[i] * 10; // Scale for stepper positions
  }
}

void applyServoAdjustments() {
  for (int i = 0; i < NUM_LEGS; i++) {
    int pulse = map(knee_angles[i], 0, 180, SERVO_MIN, SERVO_MAX);
    pca9685.setPWM(i, 0, pulse); // Set servo PWM
  }
}

void applyStepperAdjustments() {
  for (int i = 0; i < NUM_LEGS; i++) {
    steppers[i].moveTo(shoulder_positions[i]); // Set target position
    steppers[i].run(); // Execute stepper movement
  }
}
