#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <MPU6050.h>
#include <math.h>

// Create PWM driver and MPU6050 objects
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
MPU6050 mpu;

// Servo pulse limits
#define SERVO_MIN  150   // Minimum pulse length
#define SERVO_MAX  600   // Maximum pulse length

// Structure for servo control parameters
struct ServoControl {
  uint8_t pin;
  float currentAngle;
  int tallestAngle;  // Fully extended angle
  int homeAngle;     // Neutral position
  int shortestAngle; // Fully retracted angle
};

// Define the RF leg's top and bottom servos
ServoControl rfTop = {7, 55, 85, 55, 30};    // Top joint
ServoControl rfBottom = {3, 50, 22, 50, 90}; // Bottom joint

// Non-blocking update parameters
const unsigned long updateInterval = 20; // Update interval in milliseconds
unsigned long previousUpdate = 0;
const float stepIncrement = 1.0; // Step size in degrees per update

// Target angles (updated continuously from sensor data)
float targetTop = rfTop.homeAngle;
float targetBottom = rfBottom.homeAngle;

// Convert an angle (0–270°) to a PWM pulse value
int angleToPulse(float angle) {
  return map((int)angle, 0, 270, SERVO_MIN, SERVO_MAX);
}

// Initialize the MPU6050 sensor
void setupMPU6050() {
  Wire.begin();
  mpu.initialize();
  if (mpu.testConnection()){
    Serial.println("MPU6050 connection successful");
  } else {
    Serial.println("MPU6050 connection failed");
  }
}

// Read the roll angle (in degrees) from the MPU6050.
// Adjust the calculation if your sensor mounting differs.
float getRoll() {
  int16_t ax, ay, az, gx, gy, gz;
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  float roll = atan2(ay, az) * 180.0 / PI;
  return roll;
}

// Map the roll value to a target angle for a given servo.
// For a right tilt (roll > 0), move toward the "tallest" angle.
// For a left tilt (roll < 0), move toward the "shortest" angle.
float getTargetAngleForServo(const ServoControl &servo, float roll) {
  float maxTilt = 15.0;  // Maximum tilt (degrees) corresponding to full adjustment
  float tiltFactor = constrain(abs(roll), 0, maxTilt) / maxTilt;
  
  // When tilting forward (roll < 0), move toward the tallest angle.
  if (roll < 0) {
    return servo.homeAngle + (servo.tallestAngle - servo.homeAngle) * tiltFactor;
  } else {
    return servo.homeAngle + (servo.shortestAngle - servo.homeAngle) * tiltFactor;
  }
}


// Incrementally adjust the current value toward the target by a maximum step.
float approachValue(float current, float target, float step) {
  if (abs(target - current) <= step) {
    return target;
  }
  if (current < target) {
    return current + step;
  } else {
    return current - step;
  }
}

void setup() {
  Serial.begin(115200);
  
  // Initialize PWM driver and set servo frequency
  pwm.begin();
  pwm.setPWMFreq(50);
  
  // Initialize MPU6050
  setupMPU6050();
  
  // Set both servos to their home positions
  rfTop.currentAngle = rfTop.homeAngle;
  rfBottom.currentAngle = rfBottom.homeAngle;
  pwm.setPWM(rfTop.pin, 0, angleToPulse(rfTop.currentAngle));
  pwm.setPWM(rfBottom.pin, 0, angleToPulse(rfBottom.currentAngle));
  
  Serial.println("RF leg set to home position.");
}

void loop() {
  // Read the current roll angle continuously
  float roll = getRoll();
  Serial.print("Roll: ");
  Serial.println(roll);
  
  // Update target angles based on sensor reading
  targetTop = getTargetAngleForServo(rfTop, roll);
  targetBottom = getTargetAngleForServo(rfBottom, roll);
  
  // Check if it's time to update the servo positions
  unsigned long currentMillis = millis();
  if (currentMillis - previousUpdate >= updateInterval) {
    previousUpdate = currentMillis;
    
    // Gradually update the top servo angle
    rfTop.currentAngle = approachValue(rfTop.currentAngle, targetTop, stepIncrement);
    pwm.setPWM(rfTop.pin, 0, angleToPulse(rfTop.currentAngle));
    
    // Gradually update the bottom servo angle
    rfBottom.currentAngle = approachValue(rfBottom.currentAngle, targetBottom, stepIncrement);
    pwm.setPWM(rfBottom.pin, 0, angleToPulse(rfBottom.currentAngle));
    
    // Debug output
    Serial.print("RF Top Angle: ");
    Serial.print(rfTop.currentAngle);
    Serial.print("  Target: ");
    Serial.println(targetTop);
    Serial.print("RF Bottom Angle: ");
    Serial.print(rfBottom.currentAngle);
    Serial.print("  Target: ");
    Serial.println(targetBottom);
  }
}
