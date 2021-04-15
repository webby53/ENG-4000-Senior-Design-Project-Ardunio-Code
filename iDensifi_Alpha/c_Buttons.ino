// updates Menu Aspects or Top level functions of button presses
void buttonUpdate() {

  if (buttonRelease) {

    switch (pbulk) {
      case 1: { //Button 3 {Toggle}
          optionsMenu = true;
          currPos ++;
          if (currPos > numOptions) {
            currPos = 0;
            optionsMenu = false;
            line1 = F("                    ");
          }
          lcdClear();
          menuUpdate();
          break;
        }
      case 2: { //Button 1 {Toggle}
          optionsMenu = true;
          currPos --;
          if (currPos == 0) {
            optionsMenu = false;
            line1 = F("                    ");
          }
          else {
            if (currPos < 0) {
              currPos = numOptions;
            }
          }
          lcdClear();
          menuUpdate();
          break;
        }
      case 10: //Button 2 (Select)
        switch (mode) {
          case 0: {
              switch (currPos) {
                case 1: {
                    mode = 4;
                    lcdClear();
                    modeChange();
                    break;
                  }
                case 2: {
                    mode = 3;
                    lcdClear();
                    modeChange();
                    break;
                  }
                case 3:
                  break;
                case 4: {
                    mode = 2;
                    currPos = 1;
                    modeChange();
                    optionsMenu = true;
                    menuUpdate();
                    break;
                  }
              }
              break;
            }
          case 1:
            switch (currPos) {
              case 1:

                break;
              case 2:
                mode = 0;
                stage = 0;
                digitalWrite(LA_IN_1, LOW);
                dataOn(1);
                line3 = F("      RESETING      ");
                line1 = F("                    ");
                updateLCD();
                delay(13000);
                modeChange();

                break;
            }
            break;
          case 2: ///Diagnostics
            switch (currPos) {
              case 1: {
                  mode = 0;
                  modeChange();
                  menuUpdate();
                  break;
                }
              case 2: {
                  changeMaxTemp();
                  menuUpdate();
                  break;
                }
              case 3: {
                  changeBandPWM();
                  menuUpdate();
                  break;
                }
              case 4: {
                  changeActuatorPWM();
                  menuUpdate();
                  break;
                }
              case 5:
                motorChange();
                menuUpdate();
                break;
              case 6: {
                  pistonChange();
                  menuUpdate();
                  break;
                }
              case 7: {
                  autoHeat();
                  menuUpdate();
                  break;
                }
              case 8: {
                  toggleFan(1);
                  menuUpdate();
                  updateLCD();
                  break;
                }
              case 9: {
                  toggleFan(2);
                  menuUpdate();
                  updateLCD();
                  break;
                }
              case 10: {
                  togglePTC();
                  menuUpdate();
                  updateLCD();
                  break;
                }
            }
            break;
          case 3: {
              switch (currPos) {
                case 1:
                  mode = 0;
                  modeChange();
                  break;
                case 2:
                  break;
                case 3:
                  break;
                case 4:
                  break;
              }
              break;
            }
          case 4:
            switch (currPos) {
              case 2:
                mode = 1;
                modeChange();
                currPos = 0;
                break;
              case 3:
                mode = 0;
                modeChange();
                break;
            }
            break;
        }
        break;
    }

  }
}

void buttonCheck() {
  if (digitalRead(latch) == LOW) {
    if (door) {
      door = false;
      updateLCD();
    }
    button1State = 0; button2State = 0; button3State = 0;
    button1State = 0; button2State = 0; button3State = 0;
    if ( (millis() - lastDebounceTime1) > debounceDelay || (millis() - lastDebounceTime2) > debounceDelay || (millis() - lastDebounceTime3) > debounceDelay) {
      if (digitalRead(button1) == LOW) {
        button1State = 1;
        lastDebounceTime1 = millis();
      }
      if (digitalRead(button2) == LOW) {
        button2State = 1;
        lastDebounceTime2 = millis();
      }
      if (digitalRead(button3) == LOW) {
        button3State = 1;
        lastDebounceTime3 = millis();
      }
    }
  } else {
    door = true;
  }
  bulk = 10 * button2State + 2 * button1State +  button3State;
  phold = hold;

  if (bulk > 0) {
    hold ++;
    pbulk = bulk;
  }
  //Multi Button Press Check
  switch (bulk) {
    case 13: //Buttons 1, 2 and 3
      hardReset();
      delay(50);
      break;

    //    case 12: { //Buttons 1 and 2
    //     break;

    case 11: //Buttons 3 and 2

      break;

    case 3: //Buttons 1 and 3

      break;
  }
  //Checks if a button is no longer being pressed and if one was pressed previously
  buttonRelease = phold == hold and hold > 0;

  if (buttonRelease) {
    hold = 0;
    phold = 0;
    bulk = 0;
  }

}
