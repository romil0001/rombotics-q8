#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "ServoLimits.h"

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

int movementDelay = 20;   // Adjust for speed
int totalSteps = 40;      // Adjust for smoothness

enum PositionState { HOME, POSITION_ONE, POSITION_TWO };

struct ServoControl {
  uint8_t pin;
  float currentPulse;
};

ServoControl servos[8] = {
  {7, servoLimits[0].homePulse}, // RFT
  {3, servoLimits[1].homePulse}, // RFB
  {6, servoLimits[2].homePulse}, // RRT
  {2, servoLimits[3].homePulse}, // RRB
  {4, servoLimits[4].homePulse}, // LFT
  {0, servoLimits[5].homePulse}, // LFB
  {5, servoLimits[6].homePulse}, // LRT
  {1, servoLimits[7].homePulse}  // LRB
};

void moveToPosition(PositionState state) {
  float stepSize[8];
  int targetPulse[8];

  switch (state) {
    case POSITION_ONE:
      targetPulse[5] = servoLimits[5].maxPulse; // LFB High
      targetPulse[4] = servoLimits[4].minPulse; // LFT Low
      targetPulse[6] = servoLimits[6].maxPulse; // LRT Low
      targetPulse[7] = servoLimits[7].maxPulse; // LRB High
      targetPulse[0] = servoLimits[0].maxPulse; // RFT High
      targetPulse[1] = servoLimits[1].minPulse; // RFB Low
      targetPulse[2] = servoLimits[2].maxPulse; // RRT High
      targetPulse[3] = servoLimits[3].minPulse; // RRB Low
      break;

    case POSITION_TWO:
      targetPulse[5] = servoLimits[5].minPulse; // LFB Low
      targetPulse[4] = servoLimits[4].maxPulse; // LFT High
      targetPulse[6] = servoLimits[6].minPulse; // LRT High
      targetPulse[7] = servoLimits[7].minPulse; // LRB Low
      targetPulse[0] = servoLimits[0].minPulse; // RFT Low
      targetPulse[1] = servoLimits[1].maxPulse; // RFB High
      targetPulse[2] = servoLimits[2].minPulse; // RRT Low
      targetPulse[3] = servoLimits[3].maxPulse; // RRB High
      break;

    default:
      for (int i = 0; i < 8; i++) {
        targetPulse[i] = servoLimits[i].homePulse;
      }
      break;
  }

  for (int i = 0; i < 8; i++) {
    stepSize[i] = (targetPulse[i] - servos[i].currentPulse) / totalSteps;
  }

  for (int step = 0; step <= totalSteps; step++) {
    for (int i = 0; i < 8; i++) {
      servos[i].currentPulse += stepSize[i];
      pwm.setPWM(servos[i].pin, 0, (int)servos[i].currentPulse);
    }
    delay(movementDelay);
  }
}

void setup() {
  Serial.begin(115200);
  pwm.begin();
  pwm.setPWMFreq(50);

  for (int i = 0; i < 8; i++) {
    pwm.setPWM(servos[i].pin, 0, servoLimits[i].homePulse);
    servos[i].currentPulse = servoLimits[i].homePulse;
  }

  Serial.println("Robot set to home position.");
  delay(2000);

  Serial.println("Moving to position one...");
  moveToPosition(POSITION_ONE);
  delay(2000);

  Serial.println("Moving to position two...");
  moveToPosition(POSITION_TWO);
  delay(2000);

  Serial.println("Returning to home position...");
  moveToPosition(HOME);

  Serial.println("Motion complete.");
}

void loop() {
  // Future logic can be placed here
}