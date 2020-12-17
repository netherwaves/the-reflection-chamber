// CD4051B for photocells
const int cellDataPin = A0;
const int cellAddrPins[3] = { 9, 10, 11 };

// clocker
unsigned long cell_clk = 0;
unsigned long cellMove_clk[NUM_LASERS];

// multiplex array
int cellValues[NUM_LASERS];
CellState cellStates[NUM_LASERS];
double cellStatesSmooth[NUM_LASERS];
double cellStatesParasite[NUM_LASERS];
double cellAccum;


// ---- SETUP -----
void initCells() {
  pinMode(cellDataPin, INPUT);
  for (int i = 0; i < 3; i++)
    pinMode(cellAddrPins[i], OUTPUT);
}


// ----- LOOP -----
void readPhotocells() {
  if (millis() - cell_clk > 30) {
    cell_clk = millis();
    cellAccum = 0;
    
    for (int i = 0; i < NUM_LASERS; i++) {
      // filter value
      cellStatesSmooth[i] += (cellStates[i] - cellStatesSmooth[i]) * 0.1;
      cellStatesParasite[i] += (cellStatesSmooth[i] - cellStatesParasite[i]) * 0.01;
      cellAccum += cellStatesParasite[i] / NUM_LASERS / 2;
      
      // read from multiplexer
      for (int j = 0; j < 3; j++)
        digitalWrite(cellAddrPins[j], (i >> j) & 0x01);

      int cellValue = analogRead(cellDataPin);
      CellState newValue;

      // set state value
      if (cellValue <= CELL_MIN)
        newValue = OFF;
      else if (cellValue > CELL_MIN && cellValue <= CELL_MAX)
        newValue = ON;
      else if (cellValue > CELL_MAX)
        newValue = OVER;

      // reset timeout value when cell state changes
      if (newValue != cellStates[i]) {
        cellMove_clk[i] = millis();
      }
      
      cellStates[i] = newValue;
    }
  }
}
