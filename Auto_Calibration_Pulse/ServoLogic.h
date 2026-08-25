#ifndef SERVO_LOGIC_H
#define SERVO_LOGIC_H

#include "ServoLimits.h"

// Define the positions for which we want to update the servo pulses.
enum Position {
  POSITION_ONE,
  POSITION_TWO
};

// This function updates the targetPulse array based on the provided position state.
// It implements the logic where, for example, when RFT is set to its maximum pulse,
// RFB is set to its minimum pulse.
void updateServoTargets(Position state, int targetPulse[8]) {
  switch (state) {
    case POSITION_ONE:
      // Right Front pair: RFT (index 0) is max, RFB (index 1) is min.
      targetPulse[0] = servoLimits[0].maxPulse;  // RFT
      targetPulse[1] = servoLimits[1].minPulse;  // RFB

      // Right Rear pair.
      targetPulse[2] = servoLimits[2].maxPulse;  // RRT
      targetPulse[3] = servoLimits[3].minPulse;  // RRB

      // Left Front pair.
      targetPulse[4] = servoLimits[4].minPulse;  // LFT
      targetPulse[5] = servoLimits[5].maxPulse;  // LFB

      // Left Rear pair.
      targetPulse[6] = servoLimits[6].maxPulse;  // LRT
      targetPulse[7] = servoLimits[7].maxPulse;  // LRB
      break;

    case POSITION_TWO:
      // Right Front pair: RFT (index 0) is min, RFB (index 1) is max.
      targetPulse[0] = servoLimits[0].minPulse;  // RFT
      targetPulse[1] = servoLimits[1].maxPulse;  // RFB

      // Right Rear pair.
      targetPulse[2] = servoLimits[2].minPulse;  // RRT
      targetPulse[3] = servoLimits[3].maxPulse;  // RRB

      // Left Front pair.
      targetPulse[4] = servoLimits[4].maxPulse;  // LFT
      targetPulse[5] = servoLimits[5].minPulse;  // LFB

      // Left Rear pair.
      targetPulse[6] = servoLimits[6].minPulse;  // LRT
      targetPulse[7] = servoLimits[7].minPulse;  // LRB
      break;

    default:
      // Optionally handle any default state here.
      break;
  }
}

#endif // SERVO_LOGIC_H
