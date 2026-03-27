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
  {200, 245, 290}, //pwm 7 - RFT = Right front fumer 
  {185, 242, 300}, //pwm 3 - RFB = Right front Tibia
  {190, 235, 270}, //pwm 6 - RRT = Right rear fumer
  {185, 242, 300}, //pwm 2 - RRB = Right rear Tibia
  {215, 255, 300}, //pwm 4 - LFT = Left front fumer
  {185, 242, 300}, //pwm 0 - LFB = Left front Tibia
  {205, 245, 285}, //pwm 5 - LRT = Left rear fumer
  {165, 222, 280}  //pwm 1 - LRb = Left rear Tibia
};

#endif