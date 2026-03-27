#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// Fix missing debug/config symbols expected by AsyncServo
bool debug2 = false;
bool plotter = false;
int debug_servo = 0;
bool use_ramp = false;
bool amp_active = false;
void amperage_check(int dummy) {}
void set_ramp(int servoID, float baseSpeed, int r1, int r2, int r3, int r4) {}

#include "MPU6050_conf.h"
#include "NovaServos.h"
#include "AsyncServo.h"

// Servo controller and object array
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
AsyncServo* servo[TOTAL_SERVOS];

// Balancing parameters
float pitch, roll;
float balanceSensitivity = 3.0; // Tweak for more/less correction

void setup() {
  Serial.begin(115200);
  Wire.begin();

  // MPU6050 setup
  initMPU6050();
  calibrateMPU6050(gyroBias, accelBias);
  getAres();
  getGres();

  // PCA9685 setup
  pwm.begin();
  pwm.setPWMFreq(60);

  // Servo initialization
  for (int i = 0; i < TOTAL_SERVOS; i++) {
    servo[i] = new AsyncServo(&pwm, i);
    servoPos[i] = servoHome[i];
    targetPos[i] = servoHome[i];
    servoSpeed[i] = 5;
    activeServo[i] = 1;
  }

  Serial.println("Self-balancing initialized.");
}

void loop() {
  readAngles();
  applyBalanceCorrection();

  for (int i = 0; i < TOTAL_SERVOS; i++) {
    servo[i]->Update();
  }

  delay(10);
}

void readAngles() {
  readAccelData(accelCount);
  ax = (float)accelCount[0] * aRes - accelBias[0];
  ay = (float)accelCount[1] * aRes - accelBias[1];
  az = (float)accelCount[2] * aRes - accelBias[2];

  pitch = atan2(ax, sqrt(ay * ay + az * az)) * 180.0 / PI;
  roll  = atan2(ay, sqrt(ax * ax + az * az)) * 180.0 / PI;

  Serial.print("Pitch: ");
  Serial.print(pitch);
  Serial.print(" | Roll: ");
  Serial.println(roll);
}

void applyBalanceCorrection() {
  for (int leg = 0; leg < TOTAL_LEGS; leg++) {
    int femurID = servoLeg[leg][0];
    int tibiaID = servoLeg[leg][1];

    float femurCorrection = pitch * balanceSensitivity;
    float tibiaCorrection = roll * balanceSensitivity;

    targetPos[femurID] = constrain(
      servoHome[femurID] - femurCorrection,
      servoLimit[femurID][1],
      servoLimit[femurID][0]
    );

    targetPos[tibiaID] = constrain(
      servoHome[tibiaID] - tibiaCorrection,
      servoLimit[tibiaID][1],
      servoLimit[tibiaID][0]
    );

    activeServo[femurID] = 1;
    activeServo[tibiaID] = 1;
  }
}
