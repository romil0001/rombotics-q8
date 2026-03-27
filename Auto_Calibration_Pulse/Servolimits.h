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
  {190, 245, 300}, //pwm 7 - RFT = Right front fumer 
  {170, 252, 335}, //pwm 3 - RFB = Right front Tibia
  {170, 225, 280}, //pwm 6 - RRT = Right rear fumer
  {170, 252, 335}, //pwm 2 - RRB = Right rear Tibia
  {336, 391, 446}, //pwm 4 - LFT = Left front fumer
  {153, 235, 317}, //pwm 0 - LFB = Left front Tibia
  {293, 238, 183}, //pwm 5 - LRT = Left rear fumer
  {150, 232, 314}  //pwm 1 - LRb = Left rear Tibia
};

#endif