#include <Arduino.h>
// ===== CONFIGURATION =====
#define WORK_TIME 1500   // 25 minutes
#define BREAK_TIME 300   // 5 minutes
#define COMMON_CATHODE true

// Segment pins: a b c d e f g dp
int segPins[8] = {2, 3, 4, 5, 6, 7, 8, 9};

// Digit select pins: D1 D2 D3 D4
int digPins[4] = {10, 11, 12, 13};

// Inputs / Outputs
int startBtn = A0;
int resetBtn = A1;
int workAdjBtn = A2;
int breakAdjBtn = A3;
int potPin = A4;
int buzzer = A5;

// ===== DIGIT BITMASK MAP =====
byte digitMap[10] = {
  0b0111111, // 0
  0b0000110, // 1
  0b1011011, // 2
  0b1001111, // 3
  0b1100110, // 4
  0b1101101, // 5
  0b1111101, // 6
  0b0000111, // 7
  0b1111111, // 8
  0b1101111  // 9
};

// ===== STATE VARIABLES =====
unsigned long lastTick = 0;
int workTime = 25;  // default minutes
int breakTime = 5;  // default minutes
int timeLeft = workTime * 60;
bool running = false;
bool workMode = true;
bool adjusting = false; // true when changing times

// ==========================
void setup() {
  for (int i = 0; i < 8; i++) pinMode(segPins[i], OUTPUT);
  for (int i = 0; i < 4; i++) pinMode(digPins[i], OUTPUT);

  pinMode(startBtn, INPUT_PULLUP);
  pinMode(resetBtn, INPUT_PULLUP);
  pinMode(workAdjBtn, INPUT_PULLUP);
  pinMode(breakAdjBtn, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);
}
// ==========================
void loop() {

  // ----- START / PAUSE -----
  if (digitalRead(startBtn) == LOW) {
    running = !running;
    delay(300); // debounce
  }

  // ----- RESET -----
  if (digitalRead(resetBtn) == LOW) {
    running = false;
    workMode = true;
    timeLeft = WORK_TIME;
    delay(300);
  }

  // ----- TIMER -----
  if (running) {
    if (millis() - lastTick >= 1000) {
      lastTick = millis();
      timeLeft--;

      if (timeLeft <= 0) {
        tone(buzzer, 2000, 500);

        // Switch mode
        if (workMode == true) {
          workMode = false;
          timeLeft = BREAK_TIME;
        } else {
          workMode = true;
          timeLeft = WORK_TIME;
        }
      }
    }
  }

  // ----- DISPLAY -----
  displayTime(timeLeft);
}

// ==========================
// DISPLAY FUNCTIONS
// ==========================
void displayTime(int seconds) {
  int mins = seconds / 60;
  int secs = seconds % 60;

  int nums[4] = {
    mins / 10,
    mins % 10,
    secs / 10,
    secs % 10
  };

  for (int d = 0; d < 4; d++) {
    setDigit(d, nums[d]);
    delay(2);  // multiplex timing
  }
}

// ==========================
void setDigit(int d, int num) {

  // Turn all digits OFF
  for (int i = 0; i < 4; i++) {
    if (COMMON_CATHODE)
      digitalWrite(digPins[i], HIGH);
    else
      digitalWrite(digPins[i], LOW);
  }

  // Enable selected digit
  if (COMMON_CATHODE)
    digitalWrite(digPins[d], LOW);
  else
    digitalWrite(digPins[d], HIGH);

  // Set segments using bitmask
  for (int s = 0; s < 7; s++) {
    bool on = digitMap[num] & (1 << s);

    if (on) {
      digitalWrite(segPins[s], COMMON_CATHODE ? HIGH : LOW);
    } else {
      digitalWrite(segPins[s], COMMON_CATHODE ? LOW : HIGH);
    }
  }
}