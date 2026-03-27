/*
 *   NovaSM3 - Self-Balancing Lite Version
 *   Configuration for 8 servos (4 legs: femur + tibia only), ESP32 + MPU6050 + PCA9685
 */

#define TOTAL_SERVOS 8
#define TOTAL_LEGS 4

// Assign simplified servo IDs (0 to 7 for femur & tibia only)
#define RFF 6
#define RFT 3
#define LFF 4
#define LFT 0
#define RRF 7
#define RRT 2
#define LRF 5
#define LRT 1

// Assign leg names (index-based)
#define RF 0
#define LF 1
#define RR 2
#define LR 3

// Group servos into legs: [femur, tibia]
int servoLeg[TOTAL_LEGS][2] = {
  {RFF, RFT},  // Right Front
  {LFF, LFT},  // Left Front
  {RRF, RRT},  // Right Rear
  {LRF, LRT}   // Left Rear
};

// Home positions for each servo
float servoHome[TOTAL_SERVOS] = {
  227, 510,    // RF
  442, 225,    // LF
  227, 423,    // RR
  351, 213     // LR
};

// Min and max limits for each servo
float servoLimit[TOTAL_SERVOS][2] = {
  {185, 515}, {365, 607},   // RF
  {537, 207}, {370, 128},   // LF
  {236, 566}, {278, 520},   // RR
  {446, 116}, {358, 116}    // LR
};

// Control arrays
byte activeServo[TOTAL_SERVOS];
float servoPos[TOTAL_SERVOS];
float targetPos[TOTAL_SERVOS];
float servoSpeed[TOTAL_SERVOS];
