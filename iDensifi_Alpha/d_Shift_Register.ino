void dataShift(int p) {
  byte b;
  switch (p) {
    case 1: b = B10000000; break;
    case 2: b = B01000000; break;
    case 3: b = B00100000; break;
    case 4: b = B00010000; break;
    case 5: b = B00001000; break;
    case 6: b = B00000100; break;
    case 7: b = B00000010; break;
    case 8: b = B00000001; break;
  }
  dataOut = dataOut ^ b;
  shiftOut(data, clockPin, LSBFIRST, dataOut);
}

void dataOn(int p) {
  byte b;
  switch (p) {
    case 1: b = B10000000; break;
    case 2: b = B01000000; break;
    case 3: b = B00100000; break;
    case 4: b = B00010000; break;
    case 5: b = B00001000; break;
    case 6: b = B00000100; break;
    case 7: b = B00000010; break;
    case 8: b = B00000001; break;
  }
  dataOut = dataOut | b;
  shiftOut(data, clockPin, LSBFIRST, dataOut);
}

void dataOff(int p) {
  byte b;
  switch (p) {
    case 1: b = B10000000; break;
    case 2: b = B01000000; break;
    case 3: b = B00100000; break;
    case 4: b = B00010000; break;
    case 5: b = B00001000; break;
    case 6: b = B00000100; break;
    case 7: b = B00000010; break;
    case 8: b = B00000001; break;
  }
  dataOut = dataOut & ~b;
  shiftOut(data, clockPin, LSBFIRST, dataOut);
}
bool dataRead(int p){
  return bitRead(dataOut, p);
}
