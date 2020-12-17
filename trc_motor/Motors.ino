class SequencePoint {
  private:
  unsigned long start, duration, interval, clk;
  bool dir;
  bool initialized;
  
  public:
  SequencePoint() {
    initialized = false;
  }

  void createSequencePoint(unsigned long duration, unsigned long interval, bool dir) {
    this->start = millis();
    this->duration = duration;
    this->interval = interval;
    this->clk = millis();

    this->dir = dir;

    this->initialized = true;
  }
  
  bool updateMotorState(byte* motorValue) {
    // returns false when not initialized;
    // continues to pressure detection
    if (!initialized) return false;

    // beyond duration mark, uninitialize
    if (millis() - start > duration) {
      this->initialized = false;
    }

    // update motor state here
    if (millis() - clk > interval) {
      clk = millis();
      shiftMotorState(motorValue, dir);
    }

    // return true; block pressure detection
    return true;
  }
};


// 74HC595 for motors AB + CD
const int motorPinsAB[3] = { 2, 3, 4 };
const int motorPinsCD[3] = { 5, 6, 7 };
// clockers
unsigned long motor_clks[NUM_MOTORS];
unsigned long seq_timeout, seq_trig, seq_dly;
// multiplex arrays
byte motorValues[NUM_MOTORS];
byte motorShifts[NUM_MOTORS / 2];
// other
int motorSteps[NUM_MOTORS];
bool motorDirections[NUM_MOTORS];
// sequence points
SequencePoint sqPoints[NUM_MOTORS];


// ----- SETUP -----
void initMotors() {
  for (int i = 0; i < 3; i++)
    pinMode(motorPinsAB[i], OUTPUT);

  for (int i = 0; i < 3; i++)
    pinMode(motorPinsCD[i], OUTPUT);

  for (int i = 0; i < NUM_MOTORS; i++) {
    motorDirections[i] = 1;
  }
}

// ----- LOOP -----
void updateMotors() {
  motorShifts[0] = 0x00;
  motorShifts[1] = 0x00;

  for (int i = 0; i < NUM_MOTORS; i++) {
    // sequence
    if (!sqPoints[i].updateMotorState(&motorValues[i])) {
      // pressure detection
      int clockInterval = constrain(map(pressureValues[i], 5, 700, 500, 3), 3, 500);
  
      if (pressureValues[i] > 5) {
        // reset sequence timeout
        // when any sensor is used
        seq_timeout = millis();
        
        if (millis() - motor_clks[i] > clockInterval) {
          motor_clks[i] = millis();
  
          // shift motor state
          shiftMotorState(&motorValues[i], &motorDirections[i]);
        }
      }
    }

    // determine chip destination
    int chip = i / 2;
    int chipSubset = i % 2;
    // construct final 74HC595 byte
    motorShifts[chip] += motorValues[i] << (chipSubset == 0 ? 0 : 4);
  }

  // send to chips
  // 0: data
  // 1: latch
  // 2: clock
  digitalWrite(motorPinsAB[1], LOW);
  shiftOut(motorPinsAB[0], motorPinsAB[2], LSBFIRST, motorShifts[0]);
  digitalWrite(motorPinsAB[1], HIGH);

  digitalWrite(motorPinsCD[1], LOW);
  shiftOut(motorPinsCD[0], motorPinsCD[2], LSBFIRST, motorShifts[1]);
  digitalWrite(motorPinsCD[1], HIGH);
}

void triggerSequence() {
  if (millis() - seq_timeout <= 15000L) return;

  // trigger every second
  if (millis() - seq_trig > seq_dly) {
    seq_trig = millis();
    seq_dly = random() % 500 + 500;

    // generate variables
    int seqIndex = rand() % NUM_MOTORS;
    int duration = rand() % 500 + 200;
    int interval = rand() % 60 + 5;
    bool dir = rand() % 2;

    // create sequence point
    sqPoints[seqIndex].createSequencePoint(duration, interval, dir);
  }
}

void shiftMotorState(byte* value, boolean clockwise) {
  if (clockwise) {
    // shift bit to the right and go back
    *value >>= 1;
    if (*value < 0x01) *value = 0x08;

    // increment step counter
    // *steps++;
  } else {
    // shift bit to the left and go back
    *value <<= 1;
    if (*value > 0x08) *value = 0x01;

    // decrement step counter
    // *steps--;
  }

  // invert direction when max angle bound is hit
  // bipolar detection - [-MAX_ANGLE, MAX_ANGLE]
  // -----------------------------------
  // ONLY RELEVANT FOR A HYPOTHETICAL BETA VERSION
  // WHERE SENSORS/LASERS ARE WIRED ON SHAFTS
  // AND SLIP RINGS ARE OUT OF BUDGET
  // -----------------------------------
  // if (*steps >= MAX_ANGLE || *steps <= MAX_ANGLE)
  // *clockwise = !*clockwise;
}
