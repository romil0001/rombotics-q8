# Firmware Sketches

Each folder at the repository root is an independent Arduino sketch. Open one folder in the Arduino IDE (or PlatformIO) at a time.

## Recommended starting points

| Sketch | Purpose |
|--------|---------|
| **BalancePID** | Primary self-balancing firmware. MPU6050 + PCA9685, 8 servos, pitch/roll/yaw PID with mirrored top/bottom leg pairs. |
| **Calibrate** | Interactive serial tool to sweep and tune individual servo limits. |
| **Home_position** | Move all 8 servos to calibrated home angles. |
| **Auto_Calibration_Pulse** | Scripted motion between two preset poses using calibrated pulse limits. |

## Balancing & stabilization

| Sketch | Description |
|--------|-------------|
| `BalancePID/` | Modular PID balance control (recommended). Config in `pid_config.h`, limits in `ServoLimits.h`. |
| `Trail/` | Earlier 4-servo thruster PID prototype (top joints only). |
| `testtilt1/` | Nova-style async servo balancing on one configuration. |
| `stablisingrobot/` | Basic pitch/roll → servo mapping with optional stepper shoulder control. |
| `stablisingrobotpid/` | Same concept with AccelStepper and PID. |
| `servowithmpu/` | Simple MPU6050 tilt → 4 servo PWM demo. |
| `tiltwithservo/` | Adafruit MPU6050 + PCA9685 tilt response. |
| `Mpu_one_leg_success/` | Single-leg (RF) MPU-driven angle control experiment. |

## Calibration & motion presets

| Sketch | Description |
|--------|-------------|
| `Calibrate/` | Serial CLI: home, min→max sweep, next/prev servo. |
| `Auto_Calibration/` | Smooth transitions between home, tallest, and shortest poses. |
| `Auto_Calibration_Pulse/` | Pulse-width based pose sequencing (position one / two). |
| `Home_position/` | One-shot move to home, then idle. |
| `Tallest_position/` | Move all legs to fully extended pose. |
| `Shortest_position/` | Move all legs to fully retracted pose. |

## Stepper & hardware tests

| Sketch | Description |
|--------|-------------|
| `stepper/` | Basic stepper motor test. |
| `newcode/` | ESP32 four-axis stepper wiring test (X/Y/Z/A). |
| `servofromesp/` | ESP32 direct servo PWM test. |
| `Withoutpwmcode/` | Minimal servo control without PWM driver. |

## Full robot platform

| Sketch | Description |
|--------|-------------|
| `NovaSM3/` | Nova Spot-Micro clone v5.1 (Teensy 4.0). PS2 remote, gaits, OLED, MP3, RGB. Requires Teensy-specific libraries. |

## Servo naming convention

Legs use four corners: **RF** (right front), **LF** (left front), **RR** (right rear), **LR** (left rear).

Each leg has two joints:

- **T** / top / femur — upper joint (PCA9685 channels 4–7)
- **B** / bottom / tibia — lower joint (PCA9685 channels 0–3)

Example: `RFT` = right front top, `LFB` = left front bottom.

## PCA9685 channel map (BalancePID)

| Channel | Joint |
|---------|-------|
| 0 | LFB |
| 1 | LRB |
| 2 | RRB |
| 3 | RFB |
| 4 | LFT |
| 5 | LRT |
| 6 | RRT |
| 7 | RFT |
