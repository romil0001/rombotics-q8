// Libraries

#include "Wire.h"

 

// Global Varibles

// Stepper Settings

int TSTEPS = 200;

int Delay = 1000;

// Back Left

int DIRBL = 35;

int STEPBL = 34;

// Back Right

int DIRBR = 33;

int STEPBR = 32;

// Front Left

int DIRFL = 0;

int STEPFL = 0;

//Front Right

int DIRFR = 0;

int STEPFR = 0;

// Stepper Placeholders

int PreviouStep = 0;  // Remeber Previous Step

int CurrentStep = 0;  // The Current Step Location

int anglechange = 0;

int Steps = 0;

 

// Functions

// Motor Drive Back Left

int MotorBL(int Angle) {

  // Angle to Steps

  const float degreesPerStep = 360.0 / 200.0;

  int Steps = static_cast<int>(Angle / degreesPerStep);

 

  // Counter Clockwise or Clockwise Direction or 0

  if (Angle == 0) {  // Do Nothing

    // delay(50);

  } else if (Angle > 0) {  // Clockwise

    digitalWrite(DIRBL, HIGH);

    for (int i = 0; i < Steps; i++) {

      digitalWrite(STEPBL, HIGH);

      delayMicroseconds(Delay);

      digitalWrite(STEPBL, LOW);

      delayMicroseconds(Delay);

    }

  } else if (Angle < 0) {  // Counter Clockwise

    digitalWrite(DIRBL, LOW);

    Steps = Steps * -1;

    for (int i = 0; i < Steps; i++) {

      digitalWrite(STEPBL, HIGH);

      delayMicroseconds(Delay);

      digitalWrite(STEPBL, LOW);

      delayMicroseconds(Delay);

    }

  }

  Serial.println("Back Left");                    // Debug

  Serial.printf("Steps        | %3d\n", Steps);   // Debug

  Serial.printf("Anlge        | %3d\n", Angle);   // Debug

  Serial.printf("DIR          | %3d\n", DIRBR);   // Debug

  Serial.printf("StepBR       | %3d\n", STEPBR);  // Debug

  Serial.println();

  return Steps;

}

// Motor Drive Back Right

int MotorBR(int Angle) {

  // Angle to Steps

  const float degreesPerStep = 360.0 / 200.0;

  int Steps = static_cast<int>(Angle / degreesPerStep);

 

  // Counter Clockwise or Clockwise Direction or 0

  if (Angle == 0) {  // Do Nothing

    // delay(50);

  } else if (Angle > 0) {  // Clockwise

    digitalWrite(DIRBR, HIGH);

    for (int i = 0; i < Steps; i++) {

      digitalWrite(STEPBR, HIGH);

      delayMicroseconds(Delay);

      digitalWrite(STEPBR, LOW);

      delayMicroseconds(Delay);

    }

  } else if (Angle < 0) {  // Counter Clockwise

    digitalWrite(DIRBR, LOW);

    Steps = Steps * -1;

    for (int i = 0; i < Steps; i++) {

      digitalWrite(STEPBR, HIGH);

      delayMicroseconds(Delay);

      digitalWrite(STEPBR, LOW);

      delayMicroseconds(Delay);

    }

  }

  Serial.println("Back Right");                   // Debug

  Serial.printf("Steps        | %3d\n", Steps);   // Debug

  Serial.printf("Anlge        | %3d\n", Angle);   // Debug

  Serial.printf("DIR          | %3d\n", DIRBR);   // Debug

  Serial.printf("StepBR       | %3d\n", STEPBR);  // Debug

  Serial.println();

  return Steps;

}

 

// Motor Drive Front Left

int MotorFL(int Angle) {

  // Angle to Steps

  const float degreesPerStep = 360.0 / 200.0;

  int Steps = static_cast<int>(Angle / degreesPerStep);

 

  // Counter Clockwise or Clockwise Direction or 0

  if (Angle == 0) {  // Do Nothing

    // delay(50);

  } else if (Angle > 0) {  // Clockwise

    digitalWrite(DIRFL, HIGH);

    for (int i = 0; i < Steps; i++) {

      digitalWrite(STEPFL, HIGH);

      delayMicroseconds(Delay);

      digitalWrite(STEPFL, LOW);

      delayMicroseconds(Delay);

    }

  } else if (Angle < 0) {  // Counter Clockwise

    digitalWrite(DIRFL, LOW);

    Steps = Steps * -1;

    for (int i = 0; i < Steps; i++) {

      digitalWrite(STEPFL, HIGH);

      delayMicroseconds(Delay);

      digitalWrite(STEPFL, LOW);

      delayMicroseconds(Delay);

    }

  }

  Serial.println("Front Left");                   // Debug

  Serial.printf("Steps        | %3d\n", Steps);   // Debug

  Serial.printf("Anlge        | %3d\n", Angle);   // Debug

  Serial.printf("DIR          | %3d\n", DIRBR);   // Debug

  Serial.printf("StepBR       | %3d\n", STEPBR);  // Debug

  Serial.println();

  return Steps;

}

// Motor Drive Front Right

int MotorFR(int Angle) {

  // Angle to Steps

  const float degreesPerStep = 360.0 / 200.0;

  int Steps = static_cast<int>(Angle / degreesPerStep);

 

  // Counter Clockwise or Clockwise Direction or 0

  if (Angle == 0) {  // Do Nothing

    // delay(50);

  } else if (Angle > 0) {  // Clockwise

    digitalWrite(DIRFR, HIGH);

    for (int i = 0; i < Steps; i++) {

      digitalWrite(STEPFR, HIGH);

      delayMicroseconds(Delay);

      digitalWrite(STEPFR, LOW);

      delayMicroseconds(Delay);

    }

  } else if (Angle < 0) {  // Counter Clockwise

    digitalWrite(DIRFR, LOW);

    Steps = Steps * -1;

    for (int i = 0; i < Steps; i++) {

      digitalWrite(STEPFR, HIGH);

      delayMicroseconds(Delay);

      digitalWrite(STEPFR, LOW);

      delayMicroseconds(Delay);

    }

  }

  Serial.println("Front Right");                  // Debug

  Serial.printf("Steps        | %3d\n", Steps);   // Debug

  Serial.printf("Anlge        | %3d\n", Angle);   // Debug

  Serial.printf("DIR          | %3d\n", DIRBR);   // Debug

  Serial.printf("StepBR       | %3d\n", STEPBR);  // Debug

  Serial.println();

  return Steps;

}

 

// Setup

void setup() {

  // General

  Serial.begin(115200);

  Serial.println("Serial.........ON");

 

  // Stepper

  pinMode(STEPBL, OUTPUT);

  pinMode(DIRBL, OUTPUT);

  pinMode(STEPBR, OUTPUT);

  pinMode(DIRBR, OUTPUT);

  pinMode(STEPFL, OUTPUT);

  pinMode(DIRFL, OUTPUT);

  pinMode(STEPFR, OUTPUT);

  pinMode(DIRFR, OUTPUT);

  Serial.println("4 Steppers.....ON");

  Serial.println();

}

 

void loop() {

  anglechange = 180;             // How Far to move -180 to 180

  Steps = MotorBR(anglechange);  // Moves Motor

  delay(50);

}