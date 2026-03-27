#include <Arduino.h>
#include <Wire.h>
#include <MPU6050.h>
#include <Stepper.h>
#include <Servo.h>

// Define pins (adjust these to your wiring)
#define MPU6050_ADDRESS 0x68
#define STEPPER_PIN_1 2
#define STEPPER_PIN_2 3
#define STEPPER_PIN_3 4
#define STEPPER_PIN_4 5
#define SERVO_PIN_1 6
#define SERVO_PIN_2 7
#define SERVO_PIN_3 8
#define SERVO_PIN_4 9

// Create objects
MPU6050 accelgyro;
Stepper stepper1(200, STEPPER_PIN_1, STEPPER_PIN_2); // 200 steps per revolution (adjust if needed)
Stepper stepper2(200, STEPPER_PIN_3, STEPPER_PIN_4);
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

// Leg parameters
float l1 = 167.5; // mm
float l2 = 225.0; // mm
float maxAngle = 17.5; // degrees

// Balancing variables
float pitch = 0; // Current pitch angle
float targetPitch = 0; // Desired pitch angle (e.g., 0 for upright)
float kp = 5.0;  // PID gain (tune this!)
float ki = 0.1;  // PID gain (tune this!)
float kd = 0.5;  // PID gain (tune this!)
float integral = 0;
float previousError = 0;

// Gait variables (basic example)
int gaitState = 0;
unsigned long gaitTimer;
const int gaitCycleDuration = 1000; // milliseconds

void setup() {
  Serial.begin(115200);
  Wire.begin();
  accelgyro.initialize();

  if (!accelgyro.testConnection()) {
    Serial.println("MPU6050 connection failed");
    while (1);
  }

  servo1.attach(SERVO_PIN_1);
  servo2.attach(SERVO_PIN_2);
  servo3.attach(SERVO_PIN_3);
  servo4.attach(SERVO_PIN_4);
}

void loop() {
  // 1. Read IMU data
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // Calculate pitch (replace with your more accurate calculation if needed)
  pitch = atan2(-ax, sqrt(ay * ay + az * az)) * 180 / PI;

  // 2. Balancing (PID control)
  float error = targetPitch - pitch;
  integral += error * 0.01; // Assuming a 10ms loop time
  float derivative = (error - previousError) / 0.01;
  float correction = kp * error + ki * integral + kd * derivative;
  previousError = error;

  // 3. Leg Control (Kinematics and Motor Commands)
  // Convert correction to leg angles (example - adjust scaling as needed)
  float legAngle = constrain(correction * 2, -maxAngle, maxAngle); // constrain within motion range.

  // Calculate foot height based on leg angle (simplified kinematics)
  float footHeight = (l1 + l2) * sin(radians(legAngle));

  // Example: Move legs to achieve footHeight (adjust for your robot's mechanics)
  int stepperPosition1 = map(footHeight, -(l1 + l2), (l1 + l2), -200, 200); // map to steps
  int stepperPosition2 = map(footHeight, -(l1 + l2), (l1 + l2), -200, 200); // map to steps
  int stepperPosition3 = map(footHeight, -(l1 + l2), (l1 + l2), -200, 200); // map to steps
  int stepperPosition4 = map(footHeight, -(l1 + l2), (l1 + l2), -200, 200); // map to steps

  stepper1.moveTo(stepperPosition1);
  stepper1.run();
  stepper2.moveTo(stepperPosition2);
  stepper2.run();
  stepper3.moveTo(stepperPosition3);
  stepper3.run();
  stepper4.moveTo(stepperPosition4);
  stepper4.run();

  // 4. Gait (Basic example - improve this!)
  if (millis() - gaitTimer > gaitCycleDuration) {
    gaitState = (gaitState + 1) % 4;
    gaitTimer = millis();

    // Example gait logic (adjust servo positions based on gait state)
    switch (gaitState) {
      case 0:
        servo1.write(90); servo2.write(90); servo3.write(90); servo4.write(90); break;
      case 1:
        servo1.write(45); servo2.write(135); servo3.write(45); servo4.write(135); break;
      case 2:
        servo1.write(90); servo2.write(90); servo3.write(90); servo4.write(90); break;
      case 3:
        servo1.write(135); servo2.write(45); servo3.write(135); servo4.write(45); break;
    }
  }

  // 5. Debugging (Serial Monitor)
  Serial.print("Pitch: "); Serial.print(pitch);
  Serial.print(", Correction: "); Serial.print(correction);
  Serial.print(", Leg Angle: "); Serial.print(legAngle);
  Serial.print(", Foot Height: "); Serial.print(footHeight);
  Serial.println();

  delay(10); // Adjust loop delay as needed
}