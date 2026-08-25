#ifndef SERVOLIMITS_H
#define SERVOLIMITS_H

// Servo pulse width limits converted from angles (150-600)
struct ServoLimit {
  int minPulse;
  int homePulse;
  int maxPulse;
};

// Updated pulse-width limits based on servo angle conversions
ServoLimit servoLimits[8] = {
  {190, 245, 300}, // pwm 7 - RFT = Right front femur
  {170, 252, 335}, // pwm 3 - RFB = Right front tibia
  {170, 225, 280}, // pwm 6 - RRT = Right rear femur
  {170, 252, 335}, // pwm 2 - RRB = Right rear tibia
  {336, 391, 446}, // pwm 4 - LFT = Left front femur
  {153, 235, 317}, // pwm 0 - LFB = Left front tibia
  {183, 238, 293}, // pwm 5 - LRT = Left rear femur
  {150, 232, 314}  // pwm 1 - LRB = Left rear tibia
};

#endif