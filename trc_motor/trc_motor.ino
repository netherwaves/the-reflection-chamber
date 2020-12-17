// IMPORTANT: upload this sketch to the Freenove board
#include <Wire.h>
#include <time.h>
boolean startSketch = false;

// ------ CONSTANTS ----------
#define NUM_MOTORS 4
#define MAX_ANGLE 180


// global multiplex array
int pressureValues[4];


// ------ SETUP ----------
void setup() {
  initMotors();
  initPressure();

  srand(time(NULL));

  // I2C (slave) - SERIAL
  Wire.begin(4);
  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);
  Serial.begin(57600);
}

// used to time both Arduino startups
void requestEvent() {
  if (!startSketch) {
    Wire.write(1);
    startSketch = true;
    return;
  }
}


// ------ LOOP ----------
void loop() {
  if (!startSketch)
    return;
  
  // TODO: might be more efficient to do this
  // instead of calling the function 6-7 times in a single loop??
  // time = millis();
  
  // put your main code here, to run repeatedly:
  readPressure();
  updateMotors();
  triggerSequence();
}
