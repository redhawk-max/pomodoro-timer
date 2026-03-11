#include <Arduino.h>
// ===== CONFIG =====
#define WORK_TIME 1500   // 25 min
#define BREAK_TIME 300   // 5 min
#define COMMON_CATHODE true  // set false if common anode

// Segment pins a b c d e f g dp
int segPins[8] = {2,3,4,5,6,7,8,9};
// Digit pins D1 D2 D3 D4
int digPins[4] = {10,11,12,13};

int startBtn = A0;
int resetBtn = A1;
int buzzer = A2;

// Digit patterns (0–9)
byte digits[10][7] = {
  {1,1,1,1,1,1,0}, // 0
  {0,1,1,0,0,0,0}, // 1
  {1,1,0,1,1,0,1}, // 2
  {1,1,1,1,0,0,1}, // 3
  {0,1,1,0,0,1,1}, // 4
  {1,0,1,1,0,1,1}, // 5
  {1,0,1,1,1,1,1}, // 6
  {1,1,1,0,0,0,0}, // 7
  {1,1,1,1,1,1,1}, // 8
  {1,1,1,1,0,1,1}  // 9
};

unsigned long lastTick = 0;
int timeLeft = WORK_TIME;
bool running = false;
bool workMode = true;

// ==================
void setup() {
  for(int i=0;i<8;i++) pinMode(segPins[i], OUTPUT);
  for(int i=0;i<4;i++) pinMode(digPins[i], OUTPUT);

  pinMode(startBtn, INPUT_PULLUP);
  pinMode(resetBtn, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);
}

void loop() {
  if (!digitalRead(startBtn)) {
    running = !running;
    delay(300);
  }

  if (!digitalRead(resetBtn)) {
    running = false;
    workMode = true;
    timeLeft = WORK_TIME;
    delay(300);
  }

  if (running && millis() - lastTick >= 1000) {
    lastTick = millis();
    timeLeft--;

    if (timeLeft <= 0) {
      tone(buzzer, 2000, 500);
      workMode = !workMode;
      timeLeft = workMode ? WORK_TIME : BREAK_TIME;
    }
  }

  displayTime(timeLeft);
}

// ===== DISPLAY =====
void displayTime(int seconds) {
  int mins = seconds / 60;
  int secs = seconds % 60;

  int nums[4] = {
    mins / 10,
    mins % 10,
    secs / 10,
    secs % 10
  };

  for (int d=0; d<4; d++) {
    setDigit(d, nums[d]);
    delay(2);
  }
}

void setDigit(int d, int num) {
  for(int i=0;i<4;i++)
    digitalWrite(digPins[i], COMMON_CATHODE ? HIGH : LOW);

  digitalWrite(digPins[d], COMMON_CATHODE ? LOW : HIGH);

  for(int s=0;s<7;s++)
    digitalWrite(segPins[s],
      digits[num][s] ? (COMMON_CATHODE ? HIGH : LOW)
                     : (COMMON_CATHODE ? LOW : HIGH)
    );
}