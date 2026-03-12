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
int buzzer   = A2;
int potPin   = A3;
int modeBtn  = A4;
int modeLED  = A5;

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
int workTime  = 1500;   // default 25 min
int breakTime = 300;    // default 5 min
int timeLeft  = 1500;
bool running = false;
bool workMode = true;    // current running mode
bool editWork = true;    // editing mode

// ==========================
void setup() {
  for (int i = 0; i < 8; i++)
    pinMode(segPins[i], OUTPUT);
  for (int i = 0; i < 4; i++)
    pinMode(digPins[i], OUTPUT);
  pinMode(startBtn, INPUT_PULLUP);
  pinMode(resetBtn, INPUT_PULLUP);
  pinMode(modeBtn, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);
  pinMode(modeLED, OUTPUT);
  pinMode(potPin, INPUT);
}

void loop() {
  // ===== MODE BUTTON (EDGE DETECTION) =====
  static bool lastModeState = HIGH;
  bool currentModeState = digitalRead(modeBtn);
  if (lastModeState == HIGH && currentModeState == LOW && !running) {
    editWork = !editWork;
    // Immediately show correct time
    if (editWork)
      timeLeft = workTime;
    else
      timeLeft = breakTime;
  }
  lastModeState = currentModeState;
  // ===== LED INDICATOR =====
  digitalWrite(modeLED, editWork ? HIGH : LOW);
  // ===== POTENTIOMETER EDITING =====
  if (!running) {
    int potValue = analogRead(potPin);
    int minutes = map(potValue, 0, 1023, 1, 60);
    int seconds = minutes * 60;
    if (editWork) {
      workTime = seconds;
      timeLeft = workTime;
    } else {
      breakTime = seconds;
      timeLeft = breakTime;
    }
  }
  // ===== START / PAUSE BUTTON =====
  if (digitalRead(startBtn) == LOW) {
    running = !running;
    delay(300);
  }
  // ===== RESET BUTTON =====
  if (digitalRead(resetBtn) == LOW) {
    running = false;
    workMode = true;
    editWork = true;
    timeLeft = workTime;
    delay(300);
  }
  // ===== TIMER COUNTDOWN =====
  if (running) {
    if (millis() - lastTick >= 1000) {
      lastTick = millis();
      timeLeft--;
      if (timeLeft <= 0) {
        tone(buzzer, 2000, 500);
        // Switch running mode
        if (workMode) {
          workMode = false;
          timeLeft = breakTime;
        } else {
          workMode = true;
          timeLeft = workTime;
        }
      }
    }
  }

  // ----- DISPLAY -----
  displayTime(timeLeft);
}

  // ===== DISPLAY =====
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
    delay(2);
  }
}
// ==========================
void setDigit(int d, int num) {
  // Turn all digits OFF
  for (int i = 0; i < 4; i++) {
    digitalWrite(digPins[i], COMMON_CATHODE ? HIGH : LOW);
  }
  // Enable selected digit
  digitalWrite(digPins[d], COMMON_CATHODE ? LOW : HIGH);
  // Set segments
  for (int s = 0; s < 7; s++) {
    bool on = digitMap[num] & (1 << s);
    if (on)
      digitalWrite(segPins[s], COMMON_CATHODE ? HIGH : LOW);
    else
      digitalWrite(segPins[s], COMMON_CATHODE ? LOW : HIGH);
  }
}