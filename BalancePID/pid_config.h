#ifndef PID_CONFIG_H
#define PID_CONFIG_H

// --- PID control parameters for Pitch (forward/backward tilt) ---
const float kp_pitch = 1.5;
const float ki_pitch = 0.1;
const float kd_pitch = 0.2;

// --- PID control parameters for Roll (side tilt) ---
const float kp_roll = 1.5;
const float ki_roll = 0.1;
const float kd_roll = 0.2;

// --- PID control parameters for Yaw (rotation about z-axis) ---
const float kp_yaw = 1.5;
const float ki_yaw = 0.1;
const float kd_yaw = 0.2;

#endif
