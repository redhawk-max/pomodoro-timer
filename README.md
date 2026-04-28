# Pomodoro Timer using Arduino Uno

## Overview

This project is a Pomodoro timer implemented using an Arduino Uno. It allows configurable work and break durations using a potentiometer and provides real-time countdown display on a 4-digit 7-segment display. The system includes buzzer alerts and LED indicators for session transitions.

A custom PCB was designed and mounted directly on top of the Arduino Uno to integrate all external components into a compact form factor.

---

## Features

- Configurable work and break durations using potentiometer
- Start and reset functionality using push buttons
- Automatic switching between work and break sessions
- Buzzer notifications for session changes
- LED status indicator for current mode
- Real-time countdown display (MM:SS format)
- Multiplexed 4-digit 7-segment display control
- Custom PCB mounted on Arduino Uno

---

## Hardware Used

- Arduino Uno
- 4-digit 7-segment display (common cathode)
- Potentiometer (10kΩ)
- Push buttons (Start, Reset, Mode)
- Active buzzer
- LED indicator
- Resistors (220Ω for segments and LED)
- Custom PCB (Arduino Uno shield-style mount)

---

## PCB Design

The custom PCB is designed as a shield that mounts directly on top of the Arduino Uno. It connects all peripherals including the display, buttons, potentiometer, buzzer, and LED into a single integrated board.

### PCB Top View
![PCB Top View](images/pcb_top_view.jpg)

### Assembled System
![Assembled Board](images/assembled_board.jpg)

---

## Pin Mapping

### Segment Display
- D2 → a
- D3 → b
- D4 → c
- D5 → d
- D6 → e
- D7 → f
- D8 → g
- D9 → dp

### Digit Control
- D10 → Digit 1
- D11 → Digit 2
- D12 → Digit 3
- D13 → Digit 4

### Inputs / Outputs
- A0 → Start Button
- A1 → Reset Button
- A2 → Buzzer
- A3 → Potentiometer
- A4 → Mode Button
- A5 → Mode LED

---

## How It Works

1. User sets work or break duration using potentiometer
2. Mode button switches between work and break configuration
3. Start button begins countdown
4. Timer alternates automatically between work and break sessions
5. Buzzer signals transitions
6. LED indicates current mode
7. Display shows remaining time in MM:SS format

---

## Project Structure
/src
main.ino

/images
pcb_top_view.jpg
assembled_board.jpg

README.md
