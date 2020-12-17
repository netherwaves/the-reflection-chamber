// CD4051B for pressure sensors
const int pressureAddrPins[3] = { 8, 9, 10 };
const int pressureDataPin = A0;
// clocker
unsigned long pressure_clk = 0;



// ----- SETUP -----
void initPressure() {
  pinMode(pressureDataPin, INPUT);
  for (int i = 0; i < 3; i++)
    pinMode(pressureAddrPins[i], OUTPUT);
}

// ----- LOOP -----
void readPressure() {
  if (millis() - pressure_clk > 20) {
    pressure_clk = millis();

    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 3; j++)
        digitalWrite(pressureAddrPins[j], (i >> j) & 0x01);
      
      pressureValues[i] = analogRead(pressureDataPin);

      // Serial.print(pressureValues[i]);
      // Serial.print(" ");
    }
    
    // Serial.println();

    //Serial.println(pressureValues[i]);
  }
}
