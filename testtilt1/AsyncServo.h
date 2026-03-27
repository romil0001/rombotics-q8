#ifndef ASYNC_SERVO_H
#define ASYNC_SERVO_H

#include <Adafruit_PWMServoDriver.h>

class AsyncServo {
  Adafruit_PWMServoDriver* driver;
  int servoID;
  int incUnit = 1;
  unsigned long lastUpdate;

public:
  AsyncServo(Adafruit_PWMServoDriver* Driver, int ServoId) {
    driver = Driver;
    servoID = ServoId;
  }

  void Update() {
    if (!activeServo[servoID]) return;

    if (servoPos[servoID] == targetPos[servoID]) {
      activeServo[servoID] = 0;
      return;
    }

    if ((millis() - lastUpdate) > servoSpeed[servoID]) {
      lastUpdate = millis();

      // Clamp targetPos within servoLimit
      if (servoLimit[servoID][0] > servoLimit[servoID][1]) {
        if (targetPos[servoID] > servoLimit[servoID][0]) targetPos[servoID] = servoLimit[servoID][0];
        if (targetPos[servoID] < servoLimit[servoID][1]) targetPos[servoID] = servoLimit[servoID][1];
      } else {
        if (targetPos[servoID] < servoLimit[servoID][0]) targetPos[servoID] = servoLimit[servoID][0];
        if (targetPos[servoID] > servoLimit[servoID][1]) targetPos[servoID] = servoLimit[servoID][1];
      }

      // Move towards target
      if (servoPos[servoID] < targetPos[servoID]) {
        servoPos[servoID] += incUnit;
      } else if (servoPos[servoID] > targetPos[servoID]) {
        servoPos[servoID] -= incUnit;
      }

      // Update PWM output
      driver->setPWM(servoID, 0, servoPos[servoID]);

      // Optional debugging
      // Serial.print("Servo "); Serial.print(servoID);
      // Serial.print(" → "); Serial.println(servoPos[servoID]);
    }
  }
};

#endif
