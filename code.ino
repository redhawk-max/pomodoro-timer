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

// ===== DEFAULTS =====
const int DEFAULT_WORK  = 1500; // 25 min
const int DEFAULT_BREAK = 300;  // 5 min

// ===== STATE VARIABLES =====
unsigned long lastTick     = 0;
unsigned long lastBlink    = 0;
unsigned long flashStart   = 0;
bool          ledState     = LOW;
bool          flashDisplay = false;

int  workTime  = DEFAULT_WORK;
int  breakTime = DEFAULT_BREAK;
int  timeLeft  = DEFAULT_WORK;
bool running   = false;
bool workMode  = true;  // current running mode
bool editWork  = true;  // which timer is being edited

// ==========================
void setup() {
  for (int i = 0; i < 8; i++)
    pinMode(segPins[i], OUTPUT);
  for (int i = 0; i < 4; i++)
    pinMode(digPins[i], OUTPUT);
  pinMode(startBtn, INPUT_PULLUP);
  pinMode(resetBtn, INPUT_PULLUP);
  pinMode(modeBtn,  INPUT_PULLUP);
  pinMode(buzzer,   OUTPUT);
  pinMode(modeLED,  OUTPUT);
  pinMode(potPin,   INPUT);
}

// ==========================
void switchMode(bool toWork) {
  workMode = toWork;
  timeLeft = toWork ? workTime : breakTime;

  // Distinct buzzer patterns
  if (toWork) {
    // Break → Work: two short high beeps
    tone(buzzer, 2200, 150); delay(200);
    tone(buzzer, 2200, 150); delay(200);
  } else {
    // Work → Break: one long low beep
    tone(buzzer, 1000, 600); delay(650);
  }

  // Flash display blank for 300 ms
  flashDisplay = true;
  flashStart   = millis();
}

// ==========================
void updateLED() {
  if (!running) {
    // Solid = editing work, OFF = editing break
    digitalWrite(modeLED, editWork ? HIGH : LOW);
    return;
  }

  if (workMode) {
    // Work session: LED solid ON
    digitalWrite(modeLED, HIGH);
  } else {
    // Break session: LED blinks at 1 Hz
    if (millis() - lastBlink >= 500) {
      lastBlink = millis();
      ledState  = !ledState;
      digitalWrite(modeLED, ledState);
    }
  }
}
void clearDisplay() {
  for (int i = 0; i < 4; i++)
    digitalWrite(digPins[i], COMMON_CATHODE ? HIGH : LOW);
  for (int s = 0; s < 7; s++)
    digitalWrite(segPins[s], COMMON_CATHODE ? LOW : HIGH);
}
void setDigit(int d, int num) {
  // Turn all digits OFF
  for (int i = 0; i < 4; i++)
    digitalWrite(digPins[i], COMMON_CATHODE ? HIGH : LOW);
  // Enable selected digit
  digitalWrite(digPins[d], COMMON_CATHODE ? LOW : HIGH);
  // Set segments
  for (int s = 0; s < 7; s++) {
    bool on = digitMap[num] & (1 << s);
    digitalWrite(segPins[s], on ? (COMMON_CATHODE ? HIGH : LOW)
                                : (COMMON_CATHODE ? LOW  : HIGH));
  }
}
void displayTime(int seconds) {
  int mins    = seconds / 60;
  int secs    = seconds % 60;
  int nums[4] = { mins / 10, mins % 10, secs / 10, secs % 10 };
  for (int d = 0; d < 4; d++) {
    setDigit(d, nums[d]);
    delay(2);
  }
}

// ==========================
void loop() {

  // ===== MODE BUTTON (EDGE DETECTION) =====
  static bool lastModeState = HIGH;
  bool currentModeState = digitalRead(modeBtn);
  if (lastModeState == HIGH && currentModeState == LOW && !running) {
    editWork = !editWork;
    // Snap to default and preview it
    if (editWork) {
      workTime = DEFAULT_WORK;
      timeLeft = DEFAULT_WORK;
    } else {
      breakTime = DEFAULT_BREAK;
      timeLeft  = DEFAULT_BREAK;
    }
  }
  lastModeState = currentModeState;

  // ===== LED INDICATOR =====
  updateLED();

  // ===== POTENTIOMETER EDITING =====
  if (!running) {
    int potValue = analogRead(potPin);
    int minutes  = map(potValue, 0, 1023, 1, 60);
    int seconds  = minutes * 60;
    if (editWork) {
      workTime = seconds;
      timeLeft = workTime;   // preview work time on display
    } else {
      breakTime = seconds;
      timeLeft  = breakTime; // preview break time on display
    }
  }

  // ===== START / PAUSE BUTTON =====
  static bool lastStartState = HIGH;
  bool currentStartState = digitalRead(startBtn);
  if (lastStartState == HIGH && currentStartState == LOW) {
    if (!running) {
      // Always begin a fresh session from work mode
      workMode = true;
      editWork = true;
      timeLeft = workTime;
      lastTick = millis();
    }
    running = !running;
  }
  lastStartState = currentStartState;

  // ===== RESET BUTTON =====
  static bool lastResetState = HIGH;
  bool currentResetState = digitalRead(resetBtn);
  if (lastResetState == HIGH && currentResetState == LOW) {
    running      = false;
    workMode     = true;
    editWork     = true;
    timeLeft     = workTime;
    ledState     = LOW;
    flashDisplay = false;
  }
  lastResetState = currentResetState;

  // ===== TIMER COUNTDOWN =====
  if (running) {
    if (millis() - lastTick >= 1000) {
      lastTick = millis();
      timeLeft--;
      if (timeLeft <= 0) {
        switchMode(!workMode); // flip between work / break
      }
    }
  }

  // ===== DISPLAY =====
  if (flashDisplay) {
    clearDisplay();
    if (millis() - flashStart >= 300) flashDisplay = false;
  } else {
    displayTime(timeLeft);
  }
}
