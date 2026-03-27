#include <Wire.h>
#include <MPU6050_tockn.h>  // Use "MPU6050_tockn" library
#include <ESP32Servo.h>

// Initialize MPU6050 sensor
MPU6050 mpu(Wire);

// Servo objects
Servo servoFR;  // Front Right
Servo servoFL;  // Front Left
Servo servoRR;  // Rear Right
Servo servoRL;  // Rear Left

// Servo GPIO pins
#define SERVO_FR 12
#define SERVO_FL 14
#define SERVO_RR 27
#define SERVO_RL 26

// Initial servo position (centered)
int baseAngle = 90;

void setup() {
    Serial.begin(115200);
    Wire.begin(21, 22); // SDA, SCL for ESP32

    // Initialize MPU6050
    mpu.begin();
    mpu.calcGyroOffsets(true);  // Auto-calibrate

    Serial.println("MPU6050 initialized!");

    // Attach servos
    servoFR.attach(SERVO_FR);
    servoFL.attach(SERVO_FL);
    servoRR.attach(SERVO_RR);
    servoRL.attach(SERVO_RL);

    // Set servos to neutral position
    setServoAngles(baseAngle, baseAngle, baseAngle, baseAngle);
}

void loop() {
    mpu.update();  // Get sensor readings

    // Get tilt angles
    float pitch = mpu.getAngleX();  // Forward/backward tilt
    float roll  = mpu.getAngleY();  // Side-to-side tilt

    Serial.print("Pitch: "); Serial.print(pitch);
    Serial.print("  Roll: "); Serial.println(roll);

    // Adjust servos based on tilt
    stabilizeServos(pitch, roll);

    delay(50);
}

// Function to set servo angles
void setServoAngles(int fr, int fl, int rr, int rl) {
    servoFR.write(fr);
    servoFL.write(fl);
    servoRR.write(rr);
    servoRL.write(rl);
}

// Function to stabilize servos based on MPU6050 tilt
void stabilizeServos(float pitch, float roll) {
    int offsetPitch = map(pitch, -30, 30, -20, 20); // Adjust range for stability
    int offsetRoll  = map(roll, -30, 30, -20, 20);  // Adjust range for side tilt

    // Calculate new servo angles
    int fr = baseAngle - offsetPitch - offsetRoll; // Adjust FR
    int fl = baseAngle - offsetPitch + offsetRoll; // Adjust FL
    int rr = baseAngle + offsetPitch - offsetRoll; // Adjust RR
    int rl = baseAngle + offsetPitch + offsetRoll; // Adjust RL

    // Limit angles to safe range (0-180 degrees)
    fr = constrain(fr, 0, 180);
    fl = constrain(fl, 0, 180);
    rr = constrain(rr, 0, 180);
    rl = constrain(rl, 0, 180);

    // Move servos
    setServoAngles(fr, fl, rr, rl);
}
