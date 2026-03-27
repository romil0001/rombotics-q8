#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVO_MIN  150  // Min pulse length out of 4096 (approx for 0°)
#define SERVO_MAX  600  // Max pulse length out of 4096 (approx for 180°)

// Map angle to PWM pulse
int angleToPulse(int angle) {
  return map(angle, 0, 270, SERVO_MIN, SERVO_MAX);
}

void setup() {
  Serial.begin(115200);
  pwm.begin();
  pwm.setPWMFreq(50);  // Analog servos run at ~50 Hz

  // Home positions
  pwm.setPWM(7, 0, angleToPulse(58)); // Right Front Top
  pwm.setPWM(3, 0, angleToPulse(58)); // Right Front Bottom
  pwm.setPWM(6, 0, angleToPulse(50)); // Right Rear Top
  pwm.setPWM(2, 0, angleToPulse(50)); // Right Rear Bottom
  pwm.setPWM(4, 0, angleToPulse(50)); // Left Front Top
  pwm.setPWM(0, 0, angleToPulse(50)); // Left Front Bottom
  pwm.setPWM(5, 0, angleToPulse(50)); // Left Rear Top
  pwm.setPWM(1, 0, angleToPulse(45)); // Left Rear Bottom
}

void loop() {
  // Keep the servos at home position
}
