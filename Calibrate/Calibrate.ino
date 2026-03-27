#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "NovaServos.h"  // Your 8-servo config

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

int currentServo = 0;
int stepDelay = 10;
bool runningTest = false;

void setup() {
  Serial.begin(115200);
  Wire.begin();

  pwm.begin();
  pwm.setPWMFreq(60);

  Serial.println("=== Servo Calibration Tool ===");
  Serial.println("Available Commands:");
  Serial.println("h - move current servo to home");
  Serial.println("m - sweep min → max");
  Serial.println("n - next servo");
  Serial.println("p - previous servo");
  Serial.println("s - stop sweep");
  Serial.println("-----------------------------");
  printServoInfo();
}

void loop() {
  if (Serial.available()) {
    char cmd = Serial.read();

    switch (cmd) {
      case 'h':
        moveTo(currentServo, servoHome[currentServo]);
        Serial.println("Moved to home.");
        break;
      case 'm':
        runningTest = true;
        Serial.println("Sweeping min → max → home...");
        sweepServo(currentServo);
        break;
      case 'n':
        currentServo = (currentServo + 1) % TOTAL_SERVOS;
        printServoInfo();
        break;
      case 'p':
        currentServo = (currentServo - 1 + TOTAL_SERVOS) % TOTAL_SERVOS;
        printServoInfo();
        break;
      case 's':
        runningTest = false;
        Serial.println("Sweep stopped.");
        break;
    }
  }
}

void moveTo(int id, float pos) {
  pos = constrain(pos, servoLimit[id][1], servoLimit[id][0]); // apply safety bounds
  pwm.setPWM(id, 0, pos);
}

void sweepServo(int id) {
  // Sweep min to max
  for (int p = servoLimit[id][1]; p <= servoLimit[id][0]; p++) {
    if (!runningTest) return;
    moveTo(id, p);
    delay(stepDelay);
  }
  delay(500);

  // Sweep max to min
  for (int p = servoLimit[id][0]; p >= servoLimit[id][1]; p--) {
    if (!runningTest) return;
    moveTo(id, p);
    delay(stepDelay);
  }
  delay(500);

  // Return to home
  moveTo(id, servoHome[id]);
  runningTest = false;
}

void printServoInfo() {
  Serial.print("Servo ID: ");
  Serial.println(currentServo);
  Serial.print("Home: ");
  Serial.print(servoHome[currentServo]);
  Serial.print(" | Min: ");
  Serial.print(servoLimit[currentServo][1]);
  Serial.print(" | Max: ");
  Serial.println(servoLimit[currentServo][0]);
}
