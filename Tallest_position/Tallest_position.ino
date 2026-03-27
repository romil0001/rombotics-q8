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

  // Set top servos to max angle, bottom servos to min angle
  pwm.setPWM(7, 0, angleToPulse(85)); // RFT - Top
  pwm.setPWM(3, 0, angleToPulse(22)); // RFB - Bottom
  pwm.setPWM(6, 0, angleToPulse(72)); // RRT - Top
  pwm.setPWM(2, 0, angleToPulse(17)); // RRB - Bottom
  pwm.setPWM(4, 0, angleToPulse(38)); // LFT - Top
  pwm.setPWM(0, 0, angleToPulse(82)); // LFB - Bottom
  pwm.setPWM(5, 0, angleToPulse(28)); // LRT - Top
  pwm.setPWM(1, 0, angleToPulse(82)); // LRB - Bottom (min in chart)

  Serial.println("Robot is now in tallest position.");
}

void loop() {
  // Standing tall!
}
