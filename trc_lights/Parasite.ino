// CD4051B for parasite LEDs
const int pDataPin = 5;
const int pAddrPins[3] = { 6, 7, 8 };
// clocker
unsigned long parasite_clk = 0;
unsigned long parasite_dly[NUM_LASERS];

// multiplexer array
int parasiteValues[4];


// ----- SETUP -----
void initParasite() {
  pinMode(pDataPin, OUTPUT);
  for (int i = 0; i < 3; i++)
    pinMode(pAddrPins[i], OUTPUT);
}

// ----- LOOP -----
void updateParasite() {
  // generative processes here...?

  for (int i = 0; i < NUM_LASERS; i++) {
    // interval is closer to 0 when more lasers are activated
    parasite_dly[i] = 40000L;
    // global accumulator
    parasite_dly[i] *= 1.0 - (pow(cellAccum, 1.5) * 0.7);
    // cell-specific
    parasite_dly[i] *= 1.0 - (max(cellStatesParasite[i] - 1, 0) * 0.3);

    if (cellStates[i] != OFF) {
      if (millis() - cellMove_clk[i] > parasite_dly[i]) {
        // split duration into byte + scalar
        int duration = random() % 750 + 500;
        int durationScalar = 1;
        
        while (duration > 255) {
          duration /= 2;
          durationScalar *= 2;
        }
        
        Wire.beginTransmission(4);
        // 1) motor index
        Wire.write(random() % 4);
        // 2) interval
        Wire.write(random() % 30 + 10);
        // 3) interval scalar
        Wire.write(1);
        // 4) duration
        Wire.write(duration);
        // 5) duration scalar
        Wire.write(durationScalar);
        Wire.endTransmission();
  
        cellMove_clk[i] = millis();
      }
    }
  }
}

void sendParasite() {
  // TODO
  // r.i.p la remise :(
  
  if (millis() - parasite_clk > 20) {
    parasite_clk = millis();

    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 3; j++)
        digitalWrite(pAddrPins[j], (i >> j) & 0x01);

      analogWrite(pDataPin, parasiteValues[i]);
    }
  }
}
