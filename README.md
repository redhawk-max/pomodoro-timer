# Arduino Pomodoro Timer ⏱️

A hardware Pomodoro timer built with an **Arduino Uno**, a **4-digit 7-segment display**, and a **simple button interface**.
The device helps users manage productivity sessions using the **Pomodoro Technique** by alternating between work and break periods.

Users can configure:

* Work session duration
* Break session duration
* Number of Pomodoro cycles

The timer automatically switches between work and break modes and stops after the configured number of sessions.

---

# Project Overview

The Pomodoro Technique improves productivity by dividing work into focused intervals separated by short breaks.

Typical cycle:

Work → Break → Work → Break → Repeat

This project implements that workflow using a microcontroller and simple hardware components.

Features include:

* Adjustable work timer (1–60 minutes)
* Adjustable break timer (1–60 minutes)
* Adjustable number of Pomodoro sessions
* Real-time countdown using a multiplexed 7-segment display
* Buzzer alerts for session transitions
* LED indicator for work/break state
* Long-press button input for session editing
* Automatic stop after all sessions complete

---

# Hardware Components

| Component                 | Purpose                   |
| ------------------------- | ------------------------- |
| Arduino Uno               | Main microcontroller      |
| 4-digit 7-segment display | Shows countdown timer     |
| Potentiometer             | Adjust timer values       |
| Push buttons (3)          | Start, Reset, Mode        |
| Passive buzzer            | Session transition alerts |
| LED                       | Mode indicator            |
| Resistors                 | Current limiting          |
| Breadboard & jumper wires | Prototyping               |

---

# Wiring Diagram

Add a wiring diagram or circuit photo here.

Example:

```
images/wiring_diagram.png
```

You can create a wiring diagram using:

* Fritzing
* Tinkercad
* KiCad
* hand-drawn diagram

---

# System Behavior

### Button Controls

| Button             | Function                                   |
| ------------------ | ------------------------------------------ |
| Start              | Start / pause timer                        |
| Reset              | Stop timer and return to work mode         |
| Mode (short press) | Switch between editing work and break time |
| Mode (long press)  | Edit Pomodoro session count                |

---

### LED Indicator

| State            | Meaning            |
| ---------------- | ------------------ |
| LED ON           | Work session       |
| LED blinking     | Break session      |
| LED solid (idle) | Editing work time  |
| LED off (idle)   | Editing break time |

---

### Potentiometer Control

The potentiometer is used to adjust values depending on the active editing mode.

| Mode          | Adjusts                   |
| ------------- | ------------------------- |
| Edit Work     | Work duration             |
| Edit Break    | Break duration            |
| Edit Sessions | Number of Pomodoro cycles |

---

# Display Format

The 7-segment display shows the countdown in **MM:SS format**.

Example:

```
25:00 → Work session
05:00 → Break session
```

When editing sessions, the display shows:

```
0004
```

Meaning **4 Pomodoro cycles**.

---

# Software Design

The firmware is written in **Arduino C++** and uses several embedded-systems concepts:

### Multiplexed Display Control

The four digits of the display are refreshed rapidly to create the appearance of a continuous display.

### Non-blocking Timing

Timing is controlled using `millis()` instead of delay loops, allowing the system to handle:

* button input
* LED blinking
* display refresh
* timer countdown

### State Logic

The program tracks several states:

* editing work time
* editing break time
* editing session count
* running work session
* running break session

### Session Tracking

Each completed work period increments a session counter.
When the configured number of sessions is reached, the timer stops automatically.

---

# Repository Structure

```
arduino-pomodoro-timer
│
├── code
│   └── pomodoro_timer.ino
│
├── images
│   ├── wiring_diagram.png
│   ├── circuit_photo.jpg
│   └── finished_device.jpg
│
├── README.md
└── LICENSE
```

---

# Demo

Add photos or videos of the device running.

Example:

```
images/finished_device.jpg
```

Recommended demo photos:

* breadboard wiring
* timer running
* close-up of display
* final assembled device

---

# How to Run

1. Connect the hardware components according to the wiring diagram.
2. Open the Arduino sketch:

```
code/pomodoro_timer.ino
```

3. Upload the program to the Arduino Uno using the Arduino IDE.
4. Power the device and configure your timer.

---

# Skills Demonstrated

This project demonstrates experience in:

* Embedded systems programming
* Arduino development
* Digital electronics
* Hardware interfacing
* Multiplexed display control
* User input handling
* State-based firmware design

---

# Future Improvements

Possible upgrades include:

* OLED or LCD display
* Rotary encoder instead of potentiometer
* Battery-powered portable design
* Enclosure / 3D printed case
* Persistent settings stored in EEPROM
* Bluetooth mobile app integration

---

# License

This project is released under the MIT License.

---

# Author

Your Name
GitHub: https://github.com/yourusername
