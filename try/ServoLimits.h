#ifndef SERVOLIMITS_H
#define SERVOLIMITS_H

// Calibration arrays for TOP servos: {min, home, max}
// Right Front TOP servo
const int SERVO_MIN_RFT[] = {190, 245, 300}; 
// Left Front TOP servo
const int SERVO_MIN_LFT[] = {336, 391, 446};
// Right Rear TOP servo
const int SERVO_MIN_RRT[] = {170, 225, 280};
// Left Rear TOP servo
const int SERVO_MIN_LRT[] = {183, 238, 293};

// Calibration arrays for Bottom servos: {min, home, max}
// Right Front Bottom servo
const int SERVO_BOTTOM_RF[] = {170, 252, 335};
// Right Rear Bottom servo
const int SERVO_BOTTOM_RR[] = {170, 252, 335};
// Left Front Bottom servo
const int SERVO_BOTTOM_LF[] = {153, 235, 317};
// Left Rear Bottom servo
const int SERVO_BOTTOM_LR[] = {150, 232, 314};

#endif
