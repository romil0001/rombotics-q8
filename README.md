# Rombotics Q8

A quadruped robot firmware collection for an 8-servo legged platform with MPU6050-based self-balancing. The project includes calibration tools, motion presets, PID stabilization experiments, and a Nova Spot-Micro clone base platform.

## Hardware

| Component | Role |
|-----------|------|
| **MCU** | ESP32 or Teensy 4.0 (sketch-dependent) |
| **MPU6050** | 6-axis IMU for pitch, roll, and yaw |
| **PCA9685** | 16-channel I2C PWM driver for 8 servos |
| **8× servos** | 2 per leg (femur + tibia), 4 legs |
| **Stepper motors** | Optional shoulder actuators (stabilization experiments) |

### Wiring (typical BalancePID setup)

- **I2C:** SDA → GPIO 21, SCL → GPIO 22 (ESP32 defaults)
- **PCA9685 address:** `0x40`
- **MPU6050 address:** `0x68`
- Servo PWM channels are documented in [docs/SKETCHES.md](docs/SKETCHES.md)

## Quick start

1. **Install [Arduino IDE](https://www.arduino.cc/en/software)** (2.x recommended) or use PlatformIO.
2. **Add board support** for your MCU (ESP32 or Teensy).
3. **Install libraries** — vendored copies live in [`libraries/`](libraries/). Point the IDE library path here, or copy needed libraries into your global `Arduino/libraries` folder:
   - Adafruit PWM Servo Driver Library
   - Adafruit BusIO
   - MPU6050_tockn (for BalancePID)
4. **Open a sketch** — start with [`BalancePID/BalancePID.ino`](BalancePID/BalancePID.ino).
5. **Calibrate servos** — run [`Calibrate/Calibrate.ino`](Calibrate/Calibrate.ino) and update limits in `ServoLimits.h`.
6. **Upload** and open Serial Monitor at **115200 baud**.

## Project structure

```
rombotics-q8/
├── BalancePID/          ← Recommended main balancing firmware
├── Calibrate/           ← Servo calibration CLI
├── Auto_Calibration*/   ← Scripted pose sequences
├── NovaSM3/             ← Full Spot-Micro clone (Teensy)
├── libraries/           ← Vendored Arduino dependencies
└── docs/
    └── SKETCHES.md      ← Index of every sketch
```

See [docs/SKETCHES.md](docs/SKETCHES.md) for a complete sketch catalog.

## BalancePID tuning

Edit [`BalancePID/pid_config.h`](BalancePID/pid_config.h) to adjust PID gains:

```cpp
const float kp_pitch = 1.5;
const float ki_pitch = 0.1;
const float kd_pitch = 0.2;
```

Edit [`BalancePID/ServoLimits.h`](BalancePID/ServoLimits.h) with per-servo `{min, home, max}` pulse widths after calibration.

The firmware:

1. Calibrates gyro offsets on boot
2. Captures a baseline orientation as "level"
3. Runs independent PID loops on pitch, roll, and yaw
4. Applies smoothed corrections to 4 top servos
5. Mirrors bottom servos proportionally via `Helpers.h`

## Development notes

- **Case-sensitive includes:** Header filenames must match exactly on Linux/macOS (`ServoLimits.h`, not `servolimits.h`).
- **NovaSM3:** The full robot sketch targets Teensy 4.0 and requires additional libraries (PS2X, DFPlayer, etc.). Disable unused features via the `*_active` flags at the top of `NovaSM3.ino`.
- **Experimental sketches:** Folders like `stablisingrobot/` and `Trail/` are earlier prototypes kept for reference.

## License

MIT — see [LICENSE](LICENSE). Third-party libraries in `libraries/` retain their original licenses.

## Credits

- Nova Spot-Micro clone base: [Chris Locke / NovaSM3](https://github.com/cguweb-com/Arduino-Projects/tree/main/Nova-SM3)
- Custom balancing and calibration work: rombotics-q8 contributors
