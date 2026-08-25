# Rombotics Q8

**IMU-based static stabilization for a low-cost 8-DOF quadruped robot**

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![Research Docs](https://img.shields.io/badge/docs-research-green.svg)](docs/RESEARCH.md)

This repository contains the **firmware, calibration tools, and research documentation** for the Rombotics Q8 quadruped platform — an open experimental testbed for studying pose recovery using MPU6050 inertial sensing and decoupled PID control across eight servo-actuated joints.

> **For academic use:** Start with [docs/RESEARCH.md](docs/RESEARCH.md) for the research overview, methodology, and citation information.

---

## Research at a Glance

| | |
|---|---|
| **Problem** | Static pose stabilization on a low-cost quadruped using IMU feedback |
| **Approach** | Decoupled 3-axis PID (pitch / roll / yaw) with femur–tibia mirroring |
| **Platform** | 8 servos, MPU6050, PCA9685, ESP32 |
| **Primary artifact** | [`BalancePID/BalancePID.ino`](BalancePID/BalancePID.ino) |
| **Data logging** | CSV serial output via `RESEARCH_LOG` in `pid_config.h` |

```mermaid
flowchart LR
    IMU[MPU6050] --> PID[3-axis PID]
    PID --> MAP[Leg mapping]
    MAP --> MIRROR[Joint mirroring]
    MIRROR --> SRV[8 servos]
```

---

## Documentation

| Document | Audience | Contents |
|----------|----------|----------|
| [**RESEARCH.md**](docs/RESEARCH.md) | Supervisors, reviewers | Abstract, contributions, architecture |
| [**METHODOLOGY.md**](docs/METHODOLOGY.md) | Methods chapter | Control law, calibration, metrics |
| [**EXPERIMENTS.md**](docs/EXPERIMENTS.md) | Lab notebook | Phase-by-phase experimental design |
| [**REPRODUCIBILITY.md**](docs/REPRODUCIBILITY.md) | Independent replicators | Step-by-step replication guide |
| [**REFERENCES.md**](docs/REFERENCES.md) | Literature review | Bibliography and prior art |
| [**SKETCHES.md**](docs/SKETCHES.md) | Developers | Firmware catalog |

---

## Quick Start (Replication)

1. Clone the repository and configure the Arduino IDE with the vendored `libraries/` folder.
2. Calibrate joints using [`Calibrate/Calibrate.ino`](Calibrate/Calibrate.ino) → update [`BalancePID/ServoLimits.h`](BalancePID/ServoLimits.h).
3. Upload [`BalancePID/BalancePID.ino`](BalancePID/BalancePID.ino) with the robot on a level surface.
4. Enable CSV logging for experiments:
   ```cpp
   // BalancePID/pid_config.h
   #define RESEARCH_LOG 1
   ```
5. Capture serial output at 115200 baud and analyze per [REPRODUCIBILITY.md](docs/REPRODUCIBILITY.md).

Full protocol: [docs/REPRODUCIBILITY.md](docs/REPRODUCIBILITY.md)

---

## Hardware

| Component | Role |
|-----------|------|
| ESP32 / Teensy 4.0 | Microcontroller (sketch-dependent) |
| MPU6050 | 6-axis IMU (pitch, roll, yaw estimation) |
| PCA9685 | 16-channel I2C PWM driver (8 servos) |
| 8× RC servos | 2 per leg — femur + tibia |
| Stepper motors (optional) | Hybrid actuation experiments |

**I2C (BalancePID):** SDA → GPIO 21, SCL → GPIO 22 · PCA9685 @ `0x40` · MPU6050 @ `0x68`

---

## Repository Structure

```
rombotics-q8/
├── BalancePID/              Primary stabilization controller (research artifact)
├── Calibrate/               Joint calibration CLI
├── docs/
│   ├── RESEARCH.md          Research overview and contributions
│   ├── METHODOLOGY.md       Control formulation and protocols
│   ├── EXPERIMENTS.md       Experimental phases 0–6
│   ├── REPRODUCIBILITY.md   Replication guide
│   └── REFERENCES.md        Bibliography
├── CITATION.cff             Academic citation metadata
├── NovaSM3/                 Full Spot-Micro locomotion platform
└── libraries/               Vendored Arduino dependencies
```

Experimental prototypes (`Trail/`, `stablisingrobotpid/`, etc.) are retained as **ablation baselines** — see [EXPERIMENTS.md](docs/EXPERIMENTS.md).

---

## Citation

```bibtex
@software{rombotics_q8_2026,
  author    = {romil0001},
  title     = {Rombotics Q8: IMU-Based Static Stabilization for a Low-Cost Quadruped},
  year      = {2026},
  url       = {https://github.com/romil0001/rombotics-q8},
  version   = {1.0.0}
}
```

See also [CITATION.cff](CITATION.cff). Update author, affiliation, and ORCID before thesis submission.

---

## License

MIT — see [LICENSE](LICENSE). Third-party libraries in `libraries/` retain their original licenses.

**Prior art:** Nova Spot-Micro clone base by [Chris Locke](https://github.com/cguweb-com/Arduino-Projects/tree/main/Nova-SM3).
