#ifndef HELPERS_H
#define HELPERS_H

// Helper function: Compute bottom PWM value by mirroring the TOP servo's position.
// When the TOP moves above home (positive offset) the bottom will move below home proportionally,
int computeBottomPWM(int topPWM, int topMin, int topHome, int topMax,
                     int bottomMin, int bottomHome, int bottomMax) {
  int offset = topPWM - topHome;
  int bottomPWM;
  if (offset >= 0) {
    // For positive offset: map TOP's upward movement to bottom's downward movement.
    bottomPWM = bottomHome - (offset * (bottomHome - bottomMin)) / (topMax - topHome);
  } else {
    // For negative offset: map TOP's downward movement to bottom's upward movement.
    bottomPWM = bottomHome + ((-offset) * (bottomMax - bottomHome)) / (topHome - topMin);
  }
  return constrain(bottomPWM, bottomMin, bottomMax);
}

#endif
