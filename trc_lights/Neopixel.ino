// pins
const int stickPin = 12;
const int ringPin = 13;
// clocker
unsigned long stick_clk = 0;
unsigned long ring_clk = 0;
unsigned long ring_millis = 0;
// light count props
const int stickCount = 8;
const int ringCount = 16;

// NeoPixel instances
Adafruit_NeoPixel stick(stickCount, stickPin, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel ring(ringCount, ringPin, NEO_GRB + NEO_KHZ800);

// color values
uint32_t white = stick.Color(140, 140, 140);
uint32_t black = stick.Color(0, 0, 0);

// center ring props
float period = .25;
float periodSmooth = .25;
unsigned long spin_clk = 0;
float spinValues[ringCount];
int spinIndex = 0;
double spinIntensity = 0;


// ----- SETUP -----
void initNeopixel() {
  // init stick
  stick.begin();
  stick.fill(black, 0, stickCount);
  stick.show();
  // init ring
  ring.begin();
  ring.fill(black, 0, ringCount);
  ring.show();
}

// ----- LOOP -----
void updateStick() {
  if (millis() - stick_clk > 20) {
    stick_clk = millis();

    // all off
    stick.fill(black, 0, stickCount);

    int saturation, value;
    uint32_t c;
    
    for (int i = 0; i < NUM_LASERS; i++) {
      saturation = 255 * constrain(cellStatesSmooth[i] - 1, 0, 1);
      value = 140 * constrain(cellStatesSmooth[i], 0, 1);
      c = stick.ColorHSV(0, saturation, value);
      
      // send to stick
      stick.setPixelColor(i * 2, c);
      stick.setPixelColor(i * 2 + 1, c);
    }

    stick.show();
  }
}

void updateRing() {
  if (millis() - spin_clk > 100) {
    spin_clk = millis();

    // dim and reassign
    for (int i = 0; i < ringCount; i++) {
      spinValues[i] -= 0.05;
      if (i == spinIndex) spinValues[i] = 1.0;
    }

    // rotate index
    spinIndex++;
    if (spinIndex == ringCount) spinIndex = 0;
  }
  
  if (millis() - ring_clk > 20) {
    period = mapfloat(cellAccum, 0, 1, 0.5, 7);
    
    ring_millis += (millis() - ring_clk) * period * TWO_PI;
    ring_clk = millis();

    // flicker intensity variation
    int maxScale = cellAccum * 150;

    // calculate spin intensity function
    spinIntensity = constrain(-1.0 * abs(fmod(spin_clk / 1000.0, 18.0) - 9) / 3 + 2, 0, 1);

    // general
    int sinVal = sin(ring_millis / 1000.0) * 1000;
    int intensity = map(sinVal, -1000, 1000, 30, 50 + maxScale);
    int saturation = spinIntensity * 100 * (cellAccum + 1.0);
    // pixel-specific
    int hue, value;
    uint32_t c;

    for (int i = 0; i < ringCount; i++) {
      hue = (i * 3000L + millis()) % 65536;
      value = max(intensity*(1.0 - (spinIntensity * 0.8)), spinValues[i] * intensity);
      
      c = ring.ColorHSV(hue, saturation, value);
      ring.setPixelColor(i, c);
    }

    ring.show();
  }
}
