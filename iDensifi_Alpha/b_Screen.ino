


unsigned long updateTimer = 0;
short updateInterval = 350;

void updateLCD() {
  if (millis() >=  updateTimer + updateInterval) {

    if (door) {
      lcdPrint(F("********************"), 0, 0);
      lcdPrint(F("      DOOR  IS      "), 0, 1);
      lcdPrint(F("        OPEN        "), 0, 2);
      lcdPrint(F("********************"), 0, 3);
    } else {
      lcdPrint(line1, 0, 0);
      lcdPrint(line2, 0, 1);
      lcdPrint(line3, 0, 2);
      lcdPrint(line4, 0, 3);
    }
    //printToSerial();
    updateTimer = millis();
  }
}

void menuUpdate() {
  lcdClear();
  line1 = "> " + options[currPos - 1];

  //display cycle
  if (currPos == numOptions) {
    line2 = F("                    ");
  } else {
    line2 = "  " + options[currPos];
  }
  if (currPos >= numOptions - 1) {
    line3 =F( "                    ");
  } else {
    line3 ="  " + options[currPos + 1];
  }
  if (currPos >= numOptions - 2) {
    line4 =F( "                    ");
  } else {
    line4 ="  " + options[currPos + 2];
  }

}

void lcdPrint(String text, short u, short v) {
  lcd.setCursor(u, v);
  lcd.print(text);
}
void lcdClear() {
  lcd.setCursor(0, 0);
  lcdPrint(F("                    "), 0, 0);
  lcdPrint(F("                    "), 0, 1);
  lcdPrint(F("                    "), 0, 2);
  lcdPrint(F("                    "), 0, 3);
}

void modeChange() {
  lcdClear();
  optionsMenu = false;
  switch (mode) {
    case 0: {
        numOptions = numOptions1;
        for (short i = 0; i < numOptions1; i++) {
          options[i] = options1[i];
        }
        break;
      }
    case 1: {
        numOptions = numOptions2;
        for (short i = 0; i < numOptions2; i++) {
          options[i] = options2[i];
        }
        break;
      }
    case 2: {
        numOptions = numOptions3;
        for (short i = 0; i < numOptions3; i++) {
          options[i] = options3[i];
        }
        break;
      }
    case 3: {
        numOptions = numOptions4;
        for (short i = 0; i < numOptions4; i++) {
          options[i] = options4[i];
        }
        break;
      }
    case 4: {
        numOptions = numOptions5;
        for (short i = 0; i < numOptions5; i++) {
          options[i] = options5[i];
        }
        break;
      }
      
  }
}
