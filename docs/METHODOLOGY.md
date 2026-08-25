# Methodology

This document describes the sensing, control, calibration, and evaluation methodology used in the Rombotics Q8 stabilization research.

---

## 1. Coordinate Frames and State Estimation

### 1.1 IMU orientation

The MPU6050 provides triaxial accelerometer and gyroscope data. Angle estimates are computed by the `MPU6050_tockn` library using gyro integration with accelerometer correction (complementary-filter-style fusion internal to the library).

Reported angles (BalancePID firmware):

| Library output | Physical meaning |
|----------------|------------------|
| `getAngleX()` | Pitch — rotation about the lateral axis (forward/backward tilt) |
| `getAngleY()` | Roll — rotation about the longitudinal axis (side tilt) |
| `getAngleZ()` | Yaw — rotation about the vertical axis |

### 1.2 Baseline subtraction

At boot, after gyro offset calibration (`calcGyroOffsets`), the robot is held in the desired nominal pose. Current angles are stored as baselines:

\[
\phi_k = \phi_k^{\text{raw}} - \phi_0, \quad
\theta_k = \theta_k^{\text{raw}} - \theta_0, \quad
\psi_k = \psi_k^{\text{raw}} - \psi_0
\]

This makes the control target **zero deviation from the boot pose**, not absolute gravity alignment.

### 1.3 Sampling

- I2C clock: 400 kHz
- Control loop period: ~10 ms (`delay(10)` in BalancePID)
- Effective sample rate: ~100 Hz

---

## 2. Control Law

### 2.1 Error definition

The desired orientation is the upright boot pose:

\[
e_{\phi} = -\phi_k, \quad e_{\theta} = -\theta_k, \quad e_{\psi} = -\psi_k
\]

### 2.2 Discrete PID

For each axis, with sample interval \(\Delta t\):

\[
I_k = I_{k-1} + e_k \cdot \Delta t
\]

\[
D_k = \frac{e_k - e_{k-1}}{\Delta t}
\]

\[
u_k = K_p e_k + K_i I_k + K_d D_k
\]

Default gains (`BalancePID/pid_config.h`):

| Axis | \(K_p\) | \(K_i\) | \(K_d\) |
|------|---------|---------|---------|
| Pitch | 1.5 | 0.1 | 0.2 |
| Roll  | 1.5 | 0.1 | 0.2 |
| Yaw   | 1.5 | 0.1 | 0.2 |

### 2.3 Output smoothing

Raw PID outputs pass through a first-order exponential filter to reduce servo jitter:

\[
\hat{u}_k = (1 - \alpha)\,\hat{u}_{k-1} + \alpha\, u_k
\]

where \(\alpha = 0.5\) (`smoothingFactor` in BalancePID).

### 2.4 Leg-wise actuation mapping

Corrective PWM offsets are applied to the four **femur (top)** servos using a fixed sign pattern that distributes pitch, roll, and yaw corrections across the quadruped footprint:

| Joint | PWM channel | Offset formula |
|-------|-------------|----------------|
| RF femur | 7 | \(+\hat{u}_\phi + \hat{u}_\theta - \hat{u}_\psi\) |
| LF femur | 4 | \(-\hat{u}_\phi + \hat{u}_\theta - \hat{u}_\psi\) |
| RR femur | 6 | \(-\hat{u}_\phi + \hat{u}_\theta - \hat{u}_\psi\) |
| LR femur | 5 | \(+\hat{u}_\phi + \hat{u}_\theta - \hat{u}_\psi\) |

Each result is added to the joint's calibrated home pulse and clamped to `[min, max]`.

**Rationale:** Pitch correction requires opposing front/rear adjustments; roll correction requires opposing left/right adjustments; yaw applies a differential pattern across diagonal pairs.

### 2.5 Femur–tibia mirroring

Tibia (bottom) joints mirror femur motion through a piecewise-linear function (`computeBottomPWM` in `Helpers.h`):

- When the femur moves above home, the tibia moves below home proportionally.
- When the femur moves below home, the tibia moves above home proportionally.

This preserves approximate leg length and antagonistic coupling without solving full inverse kinematics — a deliberate simplification for real-time embedded control.

---

## 3. Calibration Protocol

Reproducibility requires per-joint characterization before closed-loop experiments.

### Step 1 — Mechanical limits

1. Upload `Calibrate/Calibrate.ino`.
2. Connect serial monitor at 115200 baud.
3. For each servo (channels 0–7):
   - Command `m` to sweep min → max → home.
   - Record safe `{min, home, max}` pulse widths.
4. Update `BalancePID/ServoLimits.h`.

### Step 2 — Home pose

1. Upload `Home_position/Home_position.ino`.
2. Verify all joints reach the intended standing pose.
3. Adjust home values if mechanical binding occurs.

### Step 3 — IMU zero

1. Place robot on a level surface in the home pose.
2. Upload `BalancePID/BalancePID.ino`.
3. On boot, hold the robot stationary during gyro calibration (~few seconds).
4. Record baseline pitch/roll/yaw from serial output.

### Step 4 — PID tuning

Tune one axis at a time using the Ziegler–Nichols-inspired embedded workflow:

1. Set \(K_i = K_d = 0\). Increase \(K_p\) until oscillation appears.
2. Reduce \(K_p\) by ~30%. Add \(K_d\) to damp oscillation.
3. Add small \(K_i\) only if steady-state offset persists.
4. Repeat for roll and yaw independently, then test combined operation.

Document all gain sets in your lab notebook with the corresponding commit hash.

---

## 4. Evaluation Metrics

Recommended quantitative metrics for thesis or publication:

| Metric | Definition | How to measure |
|--------|------------|----------------|
| Settling time \(t_s\) | Time to return within ±2° of nominal after step disturbance | CSV log + post-processing |
| Peak overshoot \(M_p\) | Maximum angle excursion after disturbance | CSV log |
| Steady-state error \(e_{ss}\) | Mean angle offset over last 5 s of stable window | CSV log |
| Stability margin | Maximum impulse disturbance before saturation or fall | Incremental manual testing |
| Control effort | RMS of PWM deviation from home | CSV log |

Enable research logging:

```cpp
// BalancePID/pid_config.h
#define RESEARCH_LOG 1
```

CSV columns: `time_ms, pitch, roll, yaw, pid_pitch, pid_roll, pid_yaw, smooth_pitch, smooth_roll, smooth_yaw`

---

## 5. Assumptions and Limitations

1. **Quasi-static regime** — The controller targets standing pose recovery, not dynamic gaits.
2. **Small-angle linearity** — PID operates on degree-scale errors; large disturbances may saturate servos.
3. **No foot contact model** — Ground reaction forces are not estimated; corrections are kinematic.
4. **Commodity IMU drift** — Yaw estimation drifts without magnetometer fusion; yaw PID gain is often reduced or disabled.
5. **Servo deadband and backlash** — Not explicitly modeled; smoothing partially mitigates limit cycling.

---

## 6. Alternative Approaches (Documented in Repository)

| Approach | Sketch | Trade-off |
|----------|--------|-----------|
| Direct tilt → PWM | `tiltwithservo/` | Simple, no integral action, steady-state error |
| 4-DOF femur-only PID | `Trail/` | Fewer actuators, cannot adjust leg length |
| Servo + stepper hybrid | `stablisingrobotpid/` | Higher DOF at shoulder, increased mechanical complexity |
| Full locomotion stack | `NovaSM3/` | Feature-rich, Teensy-specific, harder to isolate control variables |

These alternatives are retained as ablation baselines for comparative study.
