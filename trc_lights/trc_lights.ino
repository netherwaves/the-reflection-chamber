// IMPORTANT: upload this sketch to the Elegoo board
#include <Adafruit_NeoPixel.h>
#include <Wire.h>
boolean startSketch = false;


// ----- CONSTANTS ----------
#define NUM_LASERS 4
#define CELL_MIN 500
#define CELL_MAX 600

enum CellState { OFF, ON, OVER };


// ------ SETUP ----------
void setup() {
  initLasers();
  initParasite();
  initCells();
  initNeopixel();

  // I2C (master) - SERIAL
  Wire.begin();
  Serial.begin(57600);
}


// ------ LOOP ----------
void loop() {
  if (!startSketch) {
    delay(1000);
    
    Wire.requestFrom(4, 1);

    if (Wire.available()) {
      if (Wire.read() == 1) {
        startSketch = true;
      }
    }

    // block code execution until light controller
    // has been successfully pinged
    return;
  }

  // parasite stuff
  updateParasite();
  sendParasite();
  
  // photocells
  readPhotocells();
  // lasers
  updateLasers();

  // stick
  updateStick();
  // ring
  updateRing();
}


// ----- UTIL ------

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
 return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
