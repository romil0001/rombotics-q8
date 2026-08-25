# Experimental Design

This document maps each firmware sketch to a research phase, hypothesis, procedure, and expected observations. Use it to structure thesis chapters, lab reports, or reproducibility studies.

---

## Phase 0 — Characterization and Calibration

**Objective:** Establish safe joint operating ranges and a reproducible home pose.

| Sketch | Hypothesis | Procedure | Expected outcome |
|--------|------------|-----------|------------------|
| `Calibrate/` | Each servo has distinct usable pulse limits | Serial sweep per channel; record min/home/max | Tabulated limits in `ServoLimits.h` |
| `Home_position/` | A consistent home pose can be commanded open-loop | Upload, observe static pose | All 8 joints at defined angles, no binding |
| `Auto_Calibration/` | Smooth interpolation between poses avoids mechanical shock | Run home → tallest → shortest cycle | Continuous motion without stall |
| `Tallest_position/` | Full extension is reachable within limits | Upload and verify | Legs fully extended, stable on bench |
| `Shortest_position/` | Full retraction is reachable within limits | Upload and verify | Legs fully retracted, no collision |

**Deliverable:** Calibrated `ServoLimits.h` and documented home pose photograph/diagram.

---

## Phase 1 — Single-Leg IMU Feedback

**Objective:** Validate that IMU-derived tilt can drive corrective joint motion on one leg before scaling to four.

| Sketch | Hypothesis | Procedure | Expected outcome |
|--------|------------|-----------|------------------|
| `Mpu_one_leg_success/` | RF leg joints track IMU tilt on one axis | Tilt platform manually; observe RF servos | Correlated motion; possible steady-state error |

**Variables:** Tilt axis, `balanceSensitivity`, update interval.

**Deliverable:** Qualitative plot of joint angle vs. platform tilt for one leg.

---

## Phase 2 — Direct Mapping (No PID)

**Objective:** Establish a baseline without integral or derivative action.

| Sketch | Hypothesis | Procedure | Expected outcome |
|--------|------------|-----------|------------------|
| `servowithmpu/` | Accelerometer tilt maps directly to 4 servo PWM values | Tilt robot; observe response | Immediate response; drift and offset likely |
| `tiltwithservo/` | Adafruit fusion library improves direct mapping | Same as above with Adafruit stack | Similar; library-dependent angle quality |

**Deliverable:** Comparison note on steady-state error vs. Phase 5 PID controller.

---

## Phase 3 — Partial DOF PID (Femur Only)

**Objective:** Test whether controlling only the four femur joints is sufficient for static stability.

| Sketch | Hypothesis | Procedure | Expected outcome |
|--------|------------|-----------|------------------|
| `Trail/` | 4-DOF PID on top joints stabilizes pitch/roll/yaw | Tune kp/ki/kd; apply manual disturbances | Partial correction; leg length fixed |

**Key difference from BalancePID:** No tibia mirroring; no exponential smoothing; no baseline capture.

**Deliverable:** Side-by-side stability comparison with Phase 5.

---

## Phase 4 — Hybrid Actuation

**Objective:** Evaluate whether adding stepper motors at the shoulder improves disturbance rejection.

| Sketch | Hypothesis | Procedure | Expected outcome |
|--------|------------|-----------|------------------|
| `stablisingrobot/` | Open-loop pitch/roll mapping to servos + steppers | Tilt platform; observe both actuators | Coarse correction; no PID |
| `stablisingrobotpid/` | PID on combined servo/stepper platform improves response | Tune Kp/Ki/Kd; apply disturbances | Better than open-loop; mechanical complexity high |

**Deliverable:** Discussion of cost/benefit of hybrid actuation for quasi-static tasks.

---

## Phase 5 — Full 8-DOF Stabilization (Primary Result)

**Objective:** Demonstrate static pose recovery using decoupled 3-axis PID, smoothing, and femur–tibia mirroring.

| Sketch | Hypothesis | Procedure | Expected outcome |
|--------|------------|-----------|------------------|
| **`BalancePID/`** | 8-DOF with mirroring outperforms 4-DOF femur-only control | Standard calibration; enable `RESEARCH_LOG`; apply step disturbances | Return toward nominal pose within servo limits |

### Recommended test protocol

1. **Setup:** Level surface, robot in home pose, `RESEARCH_LOG 1`, serial capture to file.
2. **Test A — Step disturbance (pitch):** Apply 5° forward push; release; record settling time and overshoot.
3. **Test B — Step disturbance (roll):** Apply lateral push; repeat metrics.
4. **Test C — Sustained offset:** Place on slight incline (~3°); measure steady-state error with/without integral term.
5. **Test D — Saturation:** Increase disturbance until PWM hits limits; record maximum recoverable angle.

### Ablation variants (modify firmware)

| Variant | Change | Tests |
|---------|--------|-------|
| No smoothing | `smoothingFactor = 1.0` | Control effort, jitter |
| No mirroring | Hold tibia at home | Leg length change during correction |
| Yaw disabled | `kp_yaw = ki_yaw = kd_yaw = 0` | Yaw drift under disturbance |
| 4-DOF only | Use `Trail/` sketch | Direct comparison |

**Deliverable:** Time-series plots, tuning table, stability margin summary.

---

## Phase 6 — Platform Integration

**Objective:** Assess integration with a full quadruped locomotion stack.

| Sketch | Hypothesis | Procedure | Expected outcome |
|--------|------------|-----------|------------------|
| `NovaSM3/` | Stabilization logic can coexist with gait engine | Enable `mpu_active`; compare movement quality | Integration challenges documented in NovaSM3 dev notes |
| `testtilt1/` | Async servo abstraction supports balance overlay | Run with MPU6050_conf + NovaServos config | Leg-wise femur/tibia correction pattern |

**Deliverable:** Integration feasibility assessment for future dynamic stability work.

---

## Data Collection Checklist

- [ ] Git commit hash recorded
- [ ] `pid_config.h` gains documented
- [ ] `ServoLimits.h` values documented
- [ ] Serial CSV log captured (`RESEARCH_LOG 1`)
- [ ] Video of disturbance test (optional)
- [ ] Environmental notes (surface, battery voltage, ambient temperature)

---

## Suggested Thesis Chapter Mapping

| Chapter section | Source material |
|-----------------|-----------------|
| Introduction | [RESEARCH.md](RESEARCH.md) — problem statement |
| Literature review | [REFERENCES.md](REFERENCES.md) |
| System design | [RESEARCH.md](RESEARCH.md) — architecture |
| Methodology | [METHODOLOGY.md](METHODOLOGY.md) |
| Experiments | This document — Phases 0–5 |
| Results | Your CSV logs and metrics |
| Discussion | Limitations in METHODOLOGY §5 |
| Future work | [RESEARCH.md](RESEARCH.md) — future work |
