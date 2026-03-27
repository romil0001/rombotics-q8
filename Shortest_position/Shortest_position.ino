#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVO_MIN  150
#define SERVO_MAX  600

int angleToPulse(int angle) {
  return map(angle, 0, 270, SERVO_MIN, SERVO_MAX);
}

void setup() {
  Serial.begin(115200);
  pwm.begin();
  pwm.setPWMFreq(50);

  // Set top servos to min angle, bottom servos to max angle (shortest posture)
  pwm.setPWM(7, 0, angleToPulse(30)); // RFT - Top
  pwm.setPWM(3, 0, angleToPulse(90)); // RFB - Bottom
  pwm.setPWM(6, 0, angleToPulse(22)); // RRT - Top
  pwm.setPWM(2, 0, angleToPulse(82)); // RRB - Bottom
  pwm.setPWM(4, 0, angleToPulse(90)); // LFT - Top
  pwm.setPWM(0, 0, angleToPulse(25)); // LFB - Bottom
  pwm.setPWM(5, 0, angleToPulse(82)); // LRT - Top
  pwm.setPWM(1, 0, angleToPulse(17)); // LRB - Bottom

  Serial.println("Robot is now in shortest position.");
}

void loop() {
  // Staying low!
}
