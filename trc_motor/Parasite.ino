unsigned long parasite_clk = 0;

// assigned in setup,
// used for parasite events
void receiveEvent(int howMany) {
  // incoming bytes:
  // 1) motor index
  // 2) interval
  // 3) interval scalar
  // 4) duration
  // 5) duration scalar

  // motor index
  int motorIndex = Wire.read();
  // interval * interval scalar
  unsigned long interval = (long)Wire.read() * (long)Wire.read();
  // duration * duration scalar
  unsigned long duration = (long)Wire.read() * (long)Wire.read();
  // direction
  bool dir = random() % 2;

  // debugging
  Serial.print(motorIndex);
  Serial.print(" ");
  Serial.print(interval);
  Serial.print(" ");
  Serial.print(duration);
  Serial.print(" ");
  Serial.println(dir);

  // trigger sequence point
  sqPoints[motorIndex].createSequencePoint(interval, duration, dir);
  seq_timeout = millis();
}
