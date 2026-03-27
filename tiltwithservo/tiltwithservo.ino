#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_MPU6050 mpu;
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Servo pin assignments (0-15 on PCA9685)
const uint8_t LEG_PINS[] = {0, 1, 2, 3};  // Front Left, Front Right, Back Left, Back Right

// Servo parameters
const int SERVO_MIN = 150;  // Minimum pulse length out of 4096
const int SERVO_MAX = 600;  // Maximum pulse length out of 4096
const float LEG_BASE_ANGLE = 30.0;  // Base angle for leg stability
const float MAX_TILT_ANGLE = 40.0;  // Maximum tilt angle

void setup(void) {
  Serial.begin(115200);
  
  // Initialize PCA9685
  pwm.begin();
  pwm.setPWMFreq(50);  // Standard servo frequency
  
  // Initialize MPU6050
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  
  Serial.println("MPU6050 Found!");
  
  // Set up MPU6050
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
  
  // Initialize servos to zero position
  setAllLegsToZero();
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Calculate tilt angles
  float roll = atan2(a.acceleration.y, a.acceleration.z);
  float pitch = atan2(-a.acceleration.x, sqrt(a.acceleration.y*a.acceleration.y + a.acceleration.z*a.acceleration.z));

  // Convert to degrees
  roll = roll * 180.0 / PI;
  pitch = pitch * 180.0 / PI;

  // Calculate leg angles based on pitch
  float legAngle = constrain(abs(pitch), 0, MAX_TILT_ANGLE) + LEG_BASE_ANGLE;

  // Set all legs to the calculated angle
  setAllLegs(legAngle);

  // Display current angles
  Serial.print("Pitch: ");
  Serial.print(pitch);
  Serial.print("°, Leg Angle: ");
  Serial.print(legAngle);
  Serial.println("°");

  delay(50);  // Faster updates for smoother movement
}

void setAllLegs(float angle) {
  // Convert angle to servo pulse width
  int pulse = map(angle, 0, 180, SERVO_MIN, SERVO_MAX);
  
  // Set all legs to the same angle
  for (uint8_t i = 0; i < 4; i++) {
    pwm.setPWM(LEG_PINS[i], 0, pulse);
  }
}

void setAllLegsToZero() {
  // Set all legs to zero position
  for (uint8_t i = 0; i < 4; i++) {
    pwm.setPWM(LEG_PINS[i], 0, SERVO_MIN);
  }
}