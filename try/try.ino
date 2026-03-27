#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <MPU6050_tockn.h>
#include "servolimits.h"
#include "pid_config.h"
#include "helpers.h"

// Create objects for the MPU6050 and the PCA9685 PWM driver
MPU6050 mpu6050(Wire);
Adafruit_PWMServoDriver pca9685 = Adafruit_PWMServoDriver(0x40);

// Smoothing parameters (adjust smoothingFactor between 0 and 1; lower is smoother but slower)
const float smoothingFactor = 0.5;
float smoothedPIDPitch = 0;
float smoothedPIDRoll = 0;
float smoothedPIDYaw = 0;

// PID internal variables for Pitch
float integralPitch = 0;
float previousPitchError = 0;

// PID internal variables for Roll
float integralRoll = 0;
float previousRollError = 0;

// PID internal variables for Yaw
float integralYaw = 0;
float previousYawError = 0;

unsigned long previousTime = 0;

// Baseline offsets for computed angles
float baselinePitch = 0;
float baselineRoll = 0;
float baselineYaw = 0;

void setup() {
  Serial.begin(115200);

  Wire.begin();
  Wire.setClock(400000); // Increase I2C speed

  pca9685.begin();
  pca9685.setPWMFreq(50); // Typical servo frequency

  mpu6050.begin();

  // Calculate and set gyro offsets automatically
  mpu6050.calcGyroOffsets(true);

  // Allow some time for the sensor to stabilize after calibration
  delay(1000);

  // Read the current angles as the “zero” baseline
  mpu6050.update();
  baselinePitch = mpu6050.getAngleX();
  baselineRoll  = mpu6050.getAngleY();
  baselineYaw   = mpu6050.getAngleZ();

  // Optionally print baseline values for debugging
  Serial.print("Baseline Pitch: "); Serial.println(baselinePitch);
  Serial.print("Baseline Roll: "); Serial.println(baselineRoll);
  Serial.print("Baseline Yaw: "); Serial.println(baselineYaw);

  previousTime = millis();
}

void loop() {
  // Update sensor readings from the MPU6050
  mpu6050.update();
  
  // Read the angles (in degrees)
  float pitch = mpu6050.getAngleX() - baselinePitch;  // adjust by baseline
  float roll  = mpu6050.getAngleY() - baselineRoll;   // adjust by baseline
  float yaw   = mpu6050.getAngleZ() - baselineYaw;     // adjust by baseline
  
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
  
  // --- Apply exponential smoothing to PID outputs ---
  smoothedPIDPitch = (1 - smoothingFactor) * smoothedPIDPitch + smoothingFactor * pidOutputPitch;
  smoothedPIDRoll  = (1 - smoothingFactor) * smoothedPIDRoll  + smoothingFactor * pidOutputRoll;
  smoothedPIDYaw   = (1 - smoothingFactor) * smoothedPIDYaw   + smoothingFactor * pidOutputYaw;
  
  // --- Combine Smoothed PID Outputs for TOP Servos ---
  // For pitch: front servos add the correction while rear servos subtract it.
  // For roll: right servos subtract the correction while left servos add it.
  // For yaw: right servos subtract the correction while left servos add it.
  int pwmRFTop = SERVO_MIN_RFT[1] + (int)smoothedPIDPitch + (int)smoothedPIDRoll - (int)smoothedPIDYaw;  // Right Front TOP (pwm channel 7)
  int pwmLFTop = SERVO_MIN_LFT[1] - (int)smoothedPIDPitch + (int)smoothedPIDRoll - (int)smoothedPIDYaw;  // Left Front TOP  (pwm channel 4)
  int pwmRRTtop = SERVO_MIN_RRT[1] - (int)smoothedPIDPitch + (int)smoothedPIDRoll - (int)smoothedPIDYaw;  // Right Rear TOP  (pwm channel 6)
  int pwmLRTtop = SERVO_MIN_LRT[1] + (int)smoothedPIDPitch + (int)smoothedPIDRoll - (int)smoothedPIDYaw;  // Left Rear TOP   (pwm channel 5)
  
  // Constrain the TOP PWM values within each servo's allowed range
  pwmRFTop = constrain(pwmRFTop, SERVO_MIN_RFT[0], SERVO_MIN_RFT[2]);
  pwmLFTop = constrain(pwmLFTop, SERVO_MIN_LFT[0], SERVO_MIN_LFT[2]);
  pwmRRTtop = constrain(pwmRRTtop, SERVO_MIN_RRT[0], SERVO_MIN_RRT[2]);
  pwmLRTtop = constrain(pwmLRTtop, SERVO_MIN_LRT[0], SERVO_MIN_LRT[2]);
  
  // --- Calculate Bottom PWM by mirroring the corresponding TOP PWM ---
  int pwmRFBottom = computeBottomPWM(pwmRFTop,
                                     SERVO_MIN_RFT[0], SERVO_MIN_RFT[1], SERVO_MIN_RFT[2],
                                     SERVO_BOTTOM_RF[0], SERVO_BOTTOM_RF[1], SERVO_BOTTOM_RF[2]); // Right Front Bottom (pwm channel 3)
                                     
  int pwmRRBottom = computeBottomPWM(pwmRRTtop,
                                     SERVO_MIN_RRT[0], SERVO_MIN_RRT[1], SERVO_MIN_RRT[2],
                                     SERVO_BOTTOM_RR[0], SERVO_BOTTOM_RR[1], SERVO_BOTTOM_RR[2]); // Right Rear Bottom  (pwm channel 2)
                                     
  int pwmLFBottom = computeBottomPWM(pwmLFTop,
                                     SERVO_MIN_LFT[0], SERVO_MIN_LFT[1], SERVO_MIN_LFT[2],
                                     SERVO_BOTTOM_LF[0], SERVO_BOTTOM_LF[1], SERVO_BOTTOM_LF[2]); // Left Front Bottom  (pwm channel 0)
                                     
  int pwmLRBottom = computeBottomPWM(pwmLRTtop,
                                     SERVO_MIN_LRT[0], SERVO_MIN_LRT[1], SERVO_MIN_LRT[2],
                                     SERVO_BOTTOM_LR[0], SERVO_BOTTOM_LR[1], SERVO_BOTTOM_LR[2]); // Left Rear Bottom   (pwm channel 1)
  
  // Write PWM values to the TOP servos
  pca9685.setPWM(7, 0, pwmRFTop);  // Right Front TOP
  pca9685.setPWM(4, 0, pwmLFTop);  // Left Front TOP
  pca9685.setPWM(6, 0, pwmRRTtop);  // Right Rear TOP
  pca9685.setPWM(5, 0, pwmLRTtop);  // Left Rear TOP
  
  // Write PWM values to the Bottom servos
  pca9685.setPWM(3, 0, pwmRFBottom);  // Right Front Bottom
  pca9685.setPWM(2, 0, pwmRRBottom);  // Right Rear Bottom
  pca9685.setPWM(0, 0, pwmLFBottom);  // Left Front Bottom
  pca9685.setPWM(1, 0, pwmLRBottom);  // Left Rear Bottom
  
  // Optional: Debug output to the serial monitor
  Serial.print("Pitch: "); Serial.print(pitch);
  Serial.print("  Roll: "); Serial.print(roll);
  Serial.print("  Yaw: "); Serial.print(yaw);
  Serial.print("  PID Pitch: "); Serial.print(pidOutputPitch);
  Serial.print("  Smoothed Pitch: "); Serial.print(smoothedPIDPitch);
  Serial.print("  PID Roll: "); Serial.print(pidOutputRoll);
  Serial.print("  Smoothed Roll: "); Serial.print(smoothedPIDRoll);
  Serial.print("  PID Yaw: "); Serial.print(pidOutputYaw);
  Serial.print("  Smoothed Yaw: "); Serial.println(smoothedPIDYaw);
  delay(10);
}
