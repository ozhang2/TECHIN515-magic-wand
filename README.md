# TECHIN515 Magic Wand: Gesture Recognition System

This is the final project for TECHIN515 — a real-time gesture recognition system using an ESP32-based motion wand. The system captures hand movement using an onboard IMU, collects data via Python, trains a machine learning model using Edge Impulse, and enables on-device inference for gesture-controlled interaction.

---

## Features

- Real-time gesture recognition for hand-drawn "O", "V", and "Z"
- Motion data captured via ESP32 + IMU (accelerometer + gyroscope)
- Python-based data logger through serial port
- Edge Impulse model training and deployment to microcontroller
- Compact, enclosed wand hardware design with integrated power

---

## Setup Instructions

### 1. Clone this repository

```bash
git clone https://github.com/yourusername/TECHIN515-magic-wand.git
cd TECHIN515-magic-wand
