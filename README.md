# 📘 Analog Calculator using Op-Amps and Arduino

## Project Overview

The Analog Calculator is a hybrid digital-analog system designed to perform basic arithmetic operations using operational amplifier (op-amp) circuits and Arduino. Unlike conventional calculators that rely entirely on software, this project uses analog hardware for computation and Arduino for control, logic, and user interaction.

The calculator supports addition, subtraction, multiplication, and square root operations. Arduino manages user input through an IR remote, applies BODMAS logic, controls the computation flow, and displays the final result.

---

## Components Used

### Hardware
- Arduino Uno
- Operational Amplifiers (LM358 / 741)
- MCP4725 Digital-to-Analog Converter (DAC)
- IR Receiver Module
- IR Remote
- Resistors and Capacitors
- Breadboard and Jumper Wires
- LCD Display / Serial Monitor

### Software
- Arduino IDE
- IRremote Library
- Wire (I2C) Library
- Adafruit MCP4725 Library

---

## Features

- Analog computation using op-amp circuits
- Digital control using Arduino
- BODMAS rule implementation
- Wireless input using IR remote
- Accurate voltage generation using external DAC
- Real-time result display

---

## Project Functionality

1. User enters numbers and operators using an IR remote.
2. Arduino decodes the input and stores the expression.
3. BODMAS logic is applied in software.
4. Digital values are converted to analog voltages using MCP4725.
5. Op-amp circuits perform the required arithmetic operation.
6. Output voltage is read by Arduino using ADC.
7. Final result is displayed to the user.

---

## Analog Operations

| Operation      | Circuit Used              |
|---------------|---------------------------|
| Addition       | Summing Amplifier         |
| Subtraction    | Differential Amplifier    |
| Multiplication | Log-Antilog Circuit       |
| Square Root    | Nonlinear Feedback Circuit|

---

## Usage

1. Connect all components as per the circuit design.
2. Upload the Arduino code using Arduino IDE.
3. Power the system using USB or external supply.
4. Enter the expression using the IR remote.
5. View the result on Serial Monitor or LCD.

---

## Challenges

- Noise sensitivity in analog circuits
- Voltage drift and calibration issues
- Limited precision compared to digital calculators

---

## Applications

- Educational demonstration of analog computing
- Embedded systems projects
- Hybrid digital-analog system design

---

## License

This project is developed for academic and educational purposes.

