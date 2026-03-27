#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVO_MIN  150
#define SERVO_MAX  600

int movementDelay = 20;    // Speed control: lower = faster
int totalSteps = 70;       // Higher = smoother

// Position states
enum PositionState { HOME, TALLEST, SHORTEST };

// Convert angle (0–270°) to PWM pulse
int angleToPulse(float angle) {
  return map((int)angle, 0, 270, SERVO_MIN, SERVO_MAX);
}

// Define each servo: pin, currentAngle, tallestAngle, homeAngle, shortestAngle
struct ServoControl {
  uint8_t pin;
  float currentAngle;
  int tallestAngle;
  int homeAngle;
  int shortestAngle;
};

// Updated with latest shortest angles
ServoControl servos[] = {

{0, 0, 82, 60, 25},  // LFB
{1, 0, 82, 45, 17},  // LRB
{2, 0, 17, 50, 82},  // RRB
{3, 0, 22, 50, 90},  // RFB
{4, 0, 38, 60, 90},  // LFT
{5, 0, 28, 50, 82},  // LRT
{6, 0, 72, 50, 22},  // RRT
{7, 0, 85, 55, 30}   // RFT


// Smoothly move all servos together in sync to a position
void moveToPosition(PositionState state) {
  float stepSize[8];

  for (int i = 0; i < 8; i++) {
    int target;
    switch (state) {
      case TALLEST:
        target = servos[i].tallestAngle;
        break;
      case SHORTEST:
        target = servos[i].shortestAngle;
        break;
      default:
        target = servos[i].homeAngle;
        break;
    }
    stepSize[i] = (target - servos[i].currentAngle) / totalSteps;
  }

  for (int step = 0; step <= totalSteps; step++) {
    for (int i = 0; i < 8; i++) {
      servos[i].currentAngle += stepSize[i];
      pwm.setPWM(servos[i].pin, 0, angleToPulse(servos[i].currentAngle));
    }
    delay(movementDelay);
  }
}

void setup() {
  Serial.begin(115200);
  pwm.begin();
  pwm.setPWMFreq(50);

  // Move to home instantly
  for (int i = 0; i < 8; i++) {
    servos[i].currentAngle = servos[i].homeAngle;
    pwm.setPWM(servos[i].pin, 0, angleToPulse(servos[i].currentAngle));
  }

  Serial.println("Robot set to home position.");
  delay(2000); // Let it settle

  Serial.println("Moving to tallest position...");
  moveToPosition(TALLEST);

  delay(2000);

  Serial.println("Moving to shortest position...");
  moveToPosition(SHORTEST);

  delay(2000);

  Serial.println("Returning to home position...");
  moveToPosition(HOME);

  Serial.println("Motion complete.");
}

void loop() {
  // Add serial or button controls here if needed
}
