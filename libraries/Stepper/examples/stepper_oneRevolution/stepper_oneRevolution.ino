// Libraries
#include "Wire.h"
#include <QMC5883LCompass.h>
QMC5883LCompass compass;
#include <LiquidCrystal.h>
//VSS-5v, VDD-GND, VO-Middle, rs-19, rw-GND, e-23, d4-18, d5-Tx2, d6-Rx2, d7-15, A-5v, K-GND
LiquidCrystal lcd(19, 23, 18, 17, 16, 15);

// Global Varibles
// Gyro
int Left = -170;
int Back = -70;
int Right = 10;
int Front = 60;
// Stepper
const int DIR = 35;
const int STEP = 34;
const int steps_per_rev = 200;
const int Delay = 2000;
const int steps = 5;

void setup() {
  // General
  Serial.begin(115200);

  // LCD
  lcd.begin(20, 4);
  lcd.setCursor(0, 0);
  Serial.print("LCD...............ON");
  lcd.print("LCD...............ON");

  // Compass Begin
  compass.init();
  lcd.setCursor(0, 1);
  Serial.print("Compass...........ON");
  lcd.print("Compass...........ON");
  // Calibration
  compass.setCalibrationOffsets(-161.00, 436.00, -95.00);
  compass.setCalibrationScales(1.17, 0.88, 1.00);

  // Stepper Controll
  pinMode(STEP, OUTPUT);
  pinMode(DIR, OUTPUT);

  // End
  delay(5000);
  lcd.clear();
}

void loop() {
  // Compass Readings
  int a;
  compass.read();
  a = compass.getAzimuth();

  // LCD Display
  lcd.setCursor(0, 0);
  lcd.print("Lean: ");
  lcd.setCursor(7, 0);

  // Calculation of Lean
  if (a == 0) {
    lcd.print("Flat          ");  // Spaces for 20 colummes on Display
    Serial.println("Flat.............LOW");
    Serial.println(a);
  } else if (a < Left) {
    lcd.print("Front Left    ");  // Spaces for 20 colummes on Display
    Serial.println("Front Left.......LOW");
    Serial.println(a);
    digitalWrite(DIR, HIGH);  // Clockwise
    Serial.println("Spinning Clockwise...");
    for (int i = 0; i < steps; i++) {
      digitalWrite(STEP, HIGH);
      delayMicroseconds(Delay);
      digitalWrite(STEP, LOW);
      delayMicroseconds(Delay);
    }
  } else if (a < Back) {
    lcd.print("Back Left     ");  // Spaces for 20 colummes on Display
    Serial.println("Back Left........LOW");
    Serial.println(a);
  } else if (a < Right) {
    lcd.print("Back Right    ");  // Spaces for 20 colummes on Display
    Serial.println("Back Right.......LOW");
    Serial.println(a);
    digitalWrite(DIR, LOW);  // Anti-Clockwise
    Serial.println("Spinning Anti-Clockwise...");
    for (int i = 0; i < steps; i++) {
      digitalWrite(STEP, HIGH);
      delayMicroseconds(Delay);
      digitalWrite(STEP, LOW);
      delayMicroseconds(Delay);
    }
  } else if (a < Front) {
    lcd.print("Front Right   ");  // Spaces for 20 colummes on Display
    Serial.println("Front Right......LOW");
    Serial.println(a);
  } else if (a > Front) {
    lcd.print("Front Left    ");  // Spaces for 20 colummes on Display
    Serial.println("Front Left.......LOW");
    Serial.println(a);
    digitalWrite(DIR, HIGH);  // Clockwise
    Serial.println("Spinning Clockwise...");
    for (int i = 0; i < steps; i++) {
      digitalWrite(STEP, HIGH);
      delayMicroseconds(Delay);
      digitalWrite(STEP, LOW);
      delayMicroseconds(Delay);
    }
  }
  Serial.println();

  //delay(50);  // Refresh rate
  digitalWrite(STEP, LOW);
}