#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <MPU6050_tockn.h>

// Create objects for the MPU6050 and the PCA9685 PWM driver
MPU6050 mpu6050(Wire);
Adafruit_PWMServoDriver pca9685 = Adafruit_PWMServoDriver(0x40);

// Servo calibration arrays for each channel: {min, home, max}
// The "home" value (second element) is the neutral position.
const int SERVO_MIN_RFT[] = {200, 245, 290}; // Right Front Thruster (home = 245)
const int SERVO_MIN_LFT[] = {215, 255, 300}; // Left Front Thruster  (home = 255)
const int SERVO_MIN_RRT[] = {205, 245, 285}; // Right Rear Thruster  (home = 245)
const int SERVO_MIN_LRT[] = {215, 255, 300}; // Left Rear Thruster   (home = 255)

// --- PID control parameters for Pitch (forward/backward tilt) ---
float kp_pitch = 1.5;
float ki_pitch = 0.05;
float kd_pitch = 0.2;

// --- PID control parameters for Roll (side tilt) ---
float kp_roll = 1.5;
float ki_roll = 0.05;
float kd_roll = 0.2;

// --- PID control parameters for Yaw (rotation about z-axis) ---
float kp_yaw = 1.0;
float ki_yaw = 0.0;
float kd_yaw = 0.0;

// --- PID internal variables for Pitch ---
float integralPitch = 0;
float previousPitchError = 0;

// --- PID internal variables for Roll ---
float integralRoll = 0;
float previousRollError = 0;

// --- PID internal variables for Yaw ---
float integralYaw = 0;
float previousYawError = 0;

unsigned long previousTime = 0;

void setup() {
  Serial.begin(115200);
  
  Wire.begin();
  Wire.setClock(400000); // Increase I2C speed

  pca9685.begin();
  pca9685.setPWMFreq(50); // Typical servo frequency

  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);

  previousTime = millis();
}

void loop() {
  // Update sensor readings from the MPU6050
  mpu6050.update();
  
  // Read the angles (in degrees)
  float pitch = mpu6050.getAngleX(); // Pitch: forward/backward tilt
  float roll  = mpu6050.getAngleY(); // Roll: right/left tilt
  float yaw   = mpu6050.getAngleZ(); // Yaw: rotation about the vertical axis
  
  // Calculate errors (desired angle is 0° for balance)
  float pitchError = 0 - pitch;
  float rollError  = 0 - roll;
  float yawError   = 0 - yaw;
  
  // Compute time difference (dt) in seconds
  unsigned long currentTime = millis();
  float dt = (currentTime - previousTime) / 1000.0;
  previousTime = currentTime;
  
  // --- PID Calculation for Pitch ---
  integralPitch += pitchError * dt;
  float pitchDerivative = (pitchError - previousPitchError) / dt;
  float pidOutputPitch = kp_pitch * pitchError + ki_pitch * integralPitch + kd_pitch * pitchDerivative;
  previousPitchError = pitchError;
  
  // --- PID Calculation for Roll ---
  integralRoll += rollError * dt;
  float rollDerivative = (rollError - previousRollError) / dt;
  float pidOutputRoll = kp_roll * rollError + ki_roll * integralRoll + kd_roll * rollDerivative;
  previousRollError = rollError;
  
  // --- PID Calculation for Yaw ---
  integralYaw += yawError * dt;
  float yawDerivative = (yawError - previousYawError) / dt;
  float pidOutputYaw = kp_yaw * yawError + ki_yaw * integralYaw + kd_yaw * yawDerivative;
  previousYawError = yawError;
  
  // --- Combine PID Outputs for Each Servo ---
  // The following combination applies:
  //   - For pitch: front servos add the correction while rear servos subtract it.
  //   - For roll: right servos subtract the correction while left servos add it.
  //   - For yaw: right servos subtract the correction while left servos add it.
  int pwmRFT = SERVO_MIN_RFT[1] + (int)pidOutputPitch + (int)pidOutputRoll - (int)pidOutputYaw;  // Right Front Thruster
  int pwmLFT = SERVO_MIN_LFT[1] - (int)pidOutputPitch + (int)pidOutputRoll - (int)pidOutputYaw;  // Left Front Thruster
  int pwmRRT = SERVO_MIN_RRT[1] - (int)pidOutputPitch + (int)pidOutputRoll - (int)pidOutputYaw;  // Right Rear Thruster
  int pwmLRT = SERVO_MIN_LRT[1] + (int)pidOutputPitch + (int)pidOutputRoll - (int)pidOutputYaw;  // Left Rear Thruster
  
  // Constrain the PWM values within each servo's allowed range
  pwmRFT = constrain(pwmRFT, SERVO_MIN_RFT[0], SERVO_MIN_RFT[2]);
  pwmLFT = constrain(pwmLFT, SERVO_MIN_LFT[0], SERVO_MIN_LFT[2]);
  pwmRRT = constrain(pwmRRT, SERVO_MIN_RRT[0], SERVO_MIN_RRT[2]);
  pwmLRT = constrain(pwmLRT, SERVO_MIN_LRT[0], SERVO_MIN_LRT[2]);
  
  // Write PWM values to the servos (adjust channel numbers as needed)
  pca9685.setPWM(7, 0, pwmRFT);  // Right Front Thruster
  pca9685.setPWM(4, 0, pwmLFT);  // Left Front Thruster
  pca9685.setPWM(6, 0, pwmRRT);  // Right Rear Thruster
  pca9685.setPWM(5, 0, pwmLRT);  // Left Rear Thruster
  
  // Optional: Debug output to the serial monitor
  Serial.print("Pitch: "); Serial.print(pitch);
  Serial.print("  Roll: "); Serial.print(roll);
  Serial.print("  Yaw: "); Serial.print(yaw);
  Serial.print("  PID Pitch: "); Serial.print(pidOutputPitch);
  Serial.print("  PID Roll: "); Serial.print(pidOutputRoll);
  Serial.print("  PID Yaw: "); Serial.println(pidOutputYaw);
  
  delay(10);
}
