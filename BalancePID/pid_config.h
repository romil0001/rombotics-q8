#ifndef PID_CONFIG_H
#define PID_CONFIG_H

// Set to 1 to emit CSV-formatted serial data for research logging (115200 baud).
// Columns: time_ms,pitch,roll,yaw,pid_pitch,pid_roll,pid_yaw,smooth_pitch,smooth_roll,smooth_yaw
#define RESEARCH_LOG 0

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
