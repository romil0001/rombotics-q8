# References

Bibliography for quadruped stabilization, IMU-based balance control, and related hardware platforms. Format: IEEE-style short references.

---

## Quadruped Locomotion and Balance

1. M. Hutter *et al.*, "AnyMal — A highly mobile and dynamic quadrupedal robot," in *Proc. IEEE/RSJ Int. Conf. Intelligent Robots and Systems (IROS)*, 2016.

2. B. Katz, J. Di Carlo, and S. Kim, "Mini Cheetah: A platform for pushing the limits of dynamic quadruped control," in *Proc. IEEE Int. Conf. Robotics and Automation (ICRA)*, 2019.

3. G. Bledt *et al.*, "MIT Cheetah 3: Design and control of a robust, dynamic quadruped robot," in *Proc. IEEE/RSJ IROS*, 2018.

4. D. Bellicoso *et al.*, "Advances in real-world applications for legged robots," *Journal of Field Robotics*, vol. 35, no. 8, pp. 1311–1326, 2018.

---

## IMU-Based Attitude Estimation

5. S. O. H. Madgwick, "An efficient orientation filter for inertial and inertial/magnetic sensor arrays," *Report*, University of Bristol, 2010.

6. R. Mahony, T. Hamel, and J.-M. Pilmlin, "Nonlinear complementary filters on the special orthogonal group," *IEEE Trans. Automatic Control*, vol. 53, no. 5, pp. 1203–1218, 2008.

7. InvenSense Inc., "MPU-6050 Product Specification," Document PS-MPU-6050A-00, Rev. 3.4, 2013.

---

## PID and Balance Control

8. K. J. Åström and T. Hägglund, *Advanced PID Control*. ISA—The Instrumentation, Systems and Automation Society, 2006.

9. S. K. Saha, *Introduction to Robotics*, 2nd ed. McGraw-Hill, 2014. *(Kinematics and joint coupling)*

10. R. C. Dorf and R. H. Bishop, *Modern Control Systems*, 13th ed. Pearson, 2017.

---

## Low-Cost and Open-Source Quadruped Platforms

11. C. Locke, "Nova SM3 — Spot Micro clone," Arduino Projects, 2021. [Online]. Available: https://novaspotmicro.com

12. SpotMicroAI Community, "SpotMicro — Open source quadruped," GitHub repository. [Online]. Available: https://github.com/spotmicroai

13. M. F. Silva and J. Costa, "Low-cost quadruped robots: A survey," *Robotics*, vol. 12, no. 3, art. 77, 2023.

---

## Hardware Components Used in This Repository

14. Adafruit Industries, "Adafruit 16-Channel 12-bit PWM/Servo Driver — PCA9685," Product Guide, 2020.

15. N. Kolban, "MPU6050_tockn Arduino Library," GitHub. [Online]. Available: https://github.com/tockn/MPU6050_tockn

16. Espressif Systems, "ESP32 Technical Reference Manual," Version 4.9, 2023.

---

## Suggested Further Reading

- **Model predictive control for legged robots:** D. Kim *et al.*, "Design of a highly dynamic humanoid robot," *IEEE/RAS Humanoids*, 2016.
- **Zero-moment point (ZMP) stability:** M. Vukobratović and J. Stepanenko, "On the stability of anthropomorphic systems," *Mathematical Biosciences*, vol. 15, pp. 1–37, 1972.
- **Complementary filter implementation:** M. Roters, "Attitude determination using a IMU," application note, 2015.

---

## Software Dependencies (Cite When Relevant)

When publishing results obtained with this repository, also cite the specific libraries used:

| Library | Citation approach |
|---------|-------------------|
| MPU6050_tockn | Kolban (Ref. 15) or library README |
| Adafruit PWM Servo Driver | Adafruit product guide (Ref. 14) |
| NovaSM3 base | Locke (Ref. 11) |
| FastIMU / MPU6050 (legacy sketches) | Respective library authors via GitHub |

---

*Note: Update author name, institution, and thesis title in [CITATION.cff](../CITATION.cff) before submission.*
