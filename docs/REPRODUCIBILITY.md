# Reproducibility Guide

This guide enables independent replication of the Rombotics Q8 stabilization experiments. Follow the steps in order.

---

## Prerequisites

| Item | Specification |
|------|---------------|
| Arduino IDE | 2.x or PlatformIO |
| Board | ESP32 Dev Module (BalancePID) or Teensy 4.0 (NovaSM3) |
| IMU | MPU6050 breakout, I2C |
| PWM driver | PCA9685 breakout, address 0x40 |
| Servos | 8× standard RC servos (50 Hz) |
| Power | Adequate 5–6 V supply for servos (separate from MCU recommended) |
| USB serial | For monitoring and CSV data capture |

---

## Step 1 — Clone and Configure Environment

```bash
git clone https://github.com/romil0001/q8-quadruped-imu-stabilization.git
cd q8-quadruped-imu-stabilization
git checkout main   # or the release tag / commit under study
```

### Arduino IDE library path

Add the repository `libraries/` folder:

**File → Preferences → Sketchbook location** — or add via **Sketch → Include Library → Add .ZIP Library** for individual libs.

Required for BalancePID:

- `Adafruit PWM Servo Driver Library`
- `Adafruit BusIO`
- `MPU6050_tockn`

All are vendored under `libraries/`.

---

## Step 2 — Hardware Assembly

### I2C wiring (ESP32 + BalancePID)

| Signal | ESP32 pin | MPU6050 | PCA9685 |
|--------|-----------|---------|---------|
| SDA | GPIO 21 | SDA | SDA |
| SCL | GPIO 22 | SCL | SCL |
| VCC | 3.3 V | VCC | VCC |
| GND | GND | GND | GND |

Servo power: connect to PCA9685 screw terminals or external 5 V bus. **Do not power servos from the ESP32 3.3 V pin.**

### Servo channel assignment

See [SKETCHES.md](SKETCHES.md) — PCA9685 channel map.

---

## Step 3 — Joint Calibration

1. Open `Calibrate/Calibrate.ino` and upload.
2. Serial monitor: 115200 baud.
3. For each servo 0–7:
   - `n` / `p` — select servo
   - `m` — sweep min → max → home
   - `s` — stop if needed
   - Record `{min, home, max}` pulse values
4. Enter values in `BalancePID/ServoLimits.h`.

**Record:** Calibration table in lab notebook.

---

## Step 4 — Verify Home Pose

1. Upload `Home_position/Home_position.ino`.
2. Confirm mechanical clearance and symmetric stance.
3. Photograph the pose for documentation.

---

## Step 5 — Run Primary Experiment (BalancePID)

1. Edit `BalancePID/pid_config.h`:
   ```cpp
   #define RESEARCH_LOG 1   // enable CSV output
   ```
2. Place robot on level surface in home pose.
3. Upload `BalancePID/BalancePID.ino`.
4. Hold robot stationary during boot calibration (~3 s).
5. Capture serial output to file:
   ```bash
   # Linux/macOS example
   python -m serial.tools.miniterm /dev/ttyUSB0 115200 > experiment_run.csv
   ```
6. Apply standardized disturbance (e.g., 5° forward push).
7. Stop capture after 30 s of stable data.

**Record:** Git commit hash, PID gains, disturbance description, CSV file.

---

## Step 6 — Post-Processing (Example)

Python snippet for settling time from CSV:

```python
import pandas as pd
import matplotlib.pyplot as plt

# Skip header line if present; adjust column names to match CSV
cols = ["time_ms", "pitch", "roll", "yaw",
        "pid_pitch", "pid_roll", "pid_yaw",
        "smooth_pitch", "smooth_roll", "smooth_yaw"]
df = pd.read_csv("experiment_run.csv", names=cols)

# Plot pitch response
df.plot(x="time_ms", y="pitch", title="Pitch response")
plt.axhline(2, color="r", linestyle="--", label="±2° band")
plt.axhline(-2, color="r", linestyle="--")
plt.legend()
plt.savefig("pitch_response.png", dpi=150)
```

Adapt parsing if the serial monitor adds timestamps or blank lines.

---

## Step 7 — Ablation Baselines

Repeat Step 5 with comparison sketches:

| Baseline | Sketch | Purpose |
|----------|--------|---------|
| No PID | `tiltwithservo/` | Direct mapping baseline |
| 4-DOF PID | `Trail/` | Partial actuation |
| Hybrid | `stablisingrobotpid/` | Stepper augmentation |

Use identical disturbance protocol for fair comparison.

---

## Reporting Checklist

For thesis, paper, or supplementary material, include:

- [ ] Repository URL and commit hash
- [ ] Hardware bill of materials (BOM)
- [ ] Wiring diagram (photo or schematic)
- [ ] `ServoLimits.h` and `pid_config.h` from the experiment
- [ ] Raw CSV logs
- [ ] Processed plots with axis labels and units
- [ ] Video of disturbance test (recommended)
- [ ] Statement of environmental conditions

---

## Known Replication Issues

| Issue | Resolution |
|-------|------------|
| Servo jitter | Reduce `smoothingFactor` or `K_p`; verify 50 Hz PWM |
| IMU drift on yaw | Set yaw PID gains to zero; accept drift for static tests |
| Boot calibration fails | Ensure robot is stationary during `calcGyroOffsets` |
| Case-sensitive includes | Use exact filenames: `ServoLimits.h`, `Helpers.h` |
| Servo limits differ per unit | Always run Phase 0 calibration on your hardware |

---

## Contact and Issues

Report replication failures via GitHub Issues with:

- Board and library versions
- Commit hash
- Serial log excerpt
- Description of observed vs. expected behavior
