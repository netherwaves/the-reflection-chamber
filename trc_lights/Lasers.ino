// 74HC595 for lasers
// { data, latch, clock }
const int laserPins[3] = { 2, 3, 4 };
// clocker variable
unsigned long laser_clk = 0;
// multiplex array + condensed
bool laserValues[NUM_LASERS];
byte laserOut = 0x01;


// ----- SETUP -----
void initLasers() {
  for (int i = 0; i < 3; i++)
    pinMode(laserPins[i], OUTPUT);

  for (int i = 0; i < NUM_LASERS; i++) {
    laserValues[i] = true;
  }
}

// ----- LOOP -----
void updateLasers() {
  // this entire code is completely useless right now,
  // but i'll keep it here regardless
  // because it might be useful at some point...
  
  if (millis() - laser_clk > 1000) {
    laser_clk = millis();

    laserOut = 0x00;
    // bit by bit...
    for (int i = 0; i < NUM_LASERS; i++) {
      laserOut += laserValues[i] << i;
    }

    // write out to Arduino
    digitalWrite(laserPins[1], LOW);
    shiftOut(laserPins[0], laserPins[2], MSBFIRST, laserOut);
    digitalWrite(laserPins[1], HIGH);
  }
}
