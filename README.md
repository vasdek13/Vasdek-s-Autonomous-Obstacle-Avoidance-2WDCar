# Vasdek's Autonomous Obstacle Avoidance 2WD Car

## 📌 Project Overview
This project is an autonomous ground vehicle (UGV) based on **Arduino Uno**. It uses a sensor fusion approach to navigate unknown environments while avoiding obstacles and preventing falls (cliff detection).

## 🛠 Hardware Used
* **Microcontroller:** Arduino Uno R3
* **Motor Driver:** L298N Dual H-Bridge
* **Sensors:** HC-SR04 (Ultrasonic), IR Obstacle Sensors
* **Actuators:** SG90 Servo Motor, DC Gear Motors
* **Power:** 2x 18650 Li-ion Batteries (7.4V)

## ⚙️ Key Features
* **Priority Logic:** Cliff detection has higher priority than obstacle avoidance to ensure hardware safety.
* **Environmental Scanning:** 180-degree scan via Servo before choosing a safe path.
* **Balanced PWM:** Fine-tuned motor speeds to compensate for hardware asymmetry.

## 📂 Installation
1. Clone this repository.
2. Install the `Servo.h` library in your Arduino IDE.
3. Upload the `.ino` file to your Arduino Uno.
