void changeMaxTemp() {
  bool toggle = true;
  line4 = F("  Select to Return");
  line1 = F("      Max Temp      ");
  line3 = F("                    ");
  line2 = F("                    ");
  while (toggle) {
    buttonCheck();
    if (buttonRelease) {
      switch (pbulk) {
        case 1:
          if (setTemp > 30) {
            setTemp -= 10;
            line3 = "";
          }
          if (setTemp <= 30) {
            line3 = F("Temp Limit: (30-280)");
          }
          break;

        case 2:
          if (setTemp < 280) {
            setTemp += 10;
            line3 = "";
          }
          if (setTemp >= 280) {
            line3 = F("Temp Limit: (30-280)");
          }
          break;
        case 10:
          toggle = false;
      }
    }
    line2 = "     < " + String(setTemp) + " oC >";
    updateLCD();
  }
}

void changeBandPWM() {

  int pwm_menu[pwmOptions] {25,  33,  50,  66,  75,  100};
  int cursor1;
  bool toggle = true;
  line4 = F("  Select to Return  ");
  line1 = F("      Band PWM      ");
  line3 = F("                    ");
  line2 = F("                    ");
  while (toggle) {
    buttonCheck();
    if (buttonRelease) {
      switch (pbulk) {
        case 2: //up
          if (cursor1 != 0) { //beginning of pwm menu
            cursor1--;
          }
          if (cursor1 == 0) {
            cursor1 = 5;
          }
          break;
        case 1: //down
          if (cursor1 < 5) {
            cursor1++;
          }
          break;
        case 10: //select
          toggle = false;
          break;
      }
    }
    line2 = "New PWM = " + String(pwm_menu[cursor1]);
    line3 = "Current PWM = " + String(pwm) + "%" ;
    updateLCD();
  }
  pwm = int((pwm_menu[cursor1] / 100) * 255);
}
void changeActuatorPWM() {
  int pwm_menu[pwmOptions] {25,  33,  50,  66,  75,  100};
  int cursor1;
  bool toggle = true;
  line2 = F("                    ");
  line4 = F("  Select to Return  ");
  line1 = F("      Band PWM      ");
  line3 = F("                    ");

  while (toggle) {
    buttonCheck();
    if (buttonRelease) {
      switch (pbulk) {
        case 2: //up
          if (cursor1 != 0) { //beginning of pwm menu
            cursor1--;
          }
          if (cursor1 == 0) {
            cursor1 = 5;
          }
          break;
        case 1: //down
          if (cursor1 < 5) {
            cursor1++;
          }
          if (cursor1 = 5) {
            cursor1 = 0;
          }
          break;
        case 10: //select
          toggle = false;
      }
    }
    line2 = "New PWM = " + String(pwm_menu[cursor1]);
    line3 = "Current PWM = " + String(pwm_actuator) + "%" ;
    updateLCD();
  }
  pwm_actuator = int((pwm_menu[cursor1] / 100) * 255);
}

void setHeatInterval() {

}

void autoHeat() {
  bool toggle = true;
  temp = pwm;
  line4 = "  Select to Return";
  unsigned long heatTimer = millis();
  while (toggle) {
    int time = (millis() - heatTimer) / 1000;
    updateTempSensor();
    line1 = F("   || AutoHeat ||");
    line2 = "HB:" + String(round(tempc1)) + "oC | PTC:" + String(round(tempc2)) + "oC  ";
    line3 = "Time: " + String(time) + "s" + "| Set: " + String(setTemp) + "   ";
    buttonCheck();
    if (buttonRelease) {
      switch (pbulk) {
        case 10:
          toggle = false;
          break;
      }
    }
    //check status and temp over
    //if now under update status
    if (tempc1 < setTemp and temp_over) {
      temp_over = false;
      temp = pwm;
    }
    //Check if temp over
    //if so, disable heating band and update status
    if (tempc1 > setTemp and !temp_over) {
      temp = LOW;
      temp_over = true;
    }

    analogWrite(heating_band_SSR , temp);
    updateLCD();
  }
  digitalWrite(heating_band_SSR , LOW);
  temp_over = false;
  updateTempSensor();
}

void pistonChange() {
  bool toggle = true;
  int test = 0;
  updateLCD();
  line1 = F("");
  line2 = F("");
  line3 = F("");
  line4 = F("");
  lcdClear();
  updateLoadSensor();

  dataOff(6);

  delay(50);
  while (toggle) {
    buttonCheck();
    updateLoadSensor();
    bool lower;
    short l;
    switch (pbulk) {

      case 1  : {
          line2 = F("      LOWERING      ");
          if (test != pbulk and !door) {
            lower = true;
            test = pbulk;
          }
          break;
        }
      case 2: {
          line2 = F("      RAISING      ");
          if (test != pbulk and !door) {
            dataOn(1);
            test = pbulk;
          }
        }
        break;

    }
    if (buttonRelease) {
      if (pbulk == 10) {
        toggle = false;
      }
      line2 = F("  L or R to Adjust  ");
      pbulk = 0;
      test = 0;
      if (!door) {
        digitalWrite(LA_IN_1, LOW);
        dataOff(1);
      }
      lower = false;
    }
        if (force < 5) {
          l = 150;
        }
        if (force > 16) {
          l = 90;
        }
        if (force > 35) {
          l = 75;
        }
        if (force > 75) {
          l = 55;
        }
    if (force > 125) {
      lower = false;
    }
    if (lower and !door) {
      analogWrite(LA_IN_1, l);
    } else {
      analogWrite(LA_IN_1, 0);
    }


    line4 = F("  Select to Return");
    line1 = F(" <Piston Adustment>");
    line3 = "    Force: " + String(force);
    updateLCD();
  }
  dataOn(6);
}

void toggleFan(int i) {
  int num = 0;
  switch (i) {
    case 1: {
        dataShift(4);
        fan1 = dataRead(4);
        num = numOptions3 - 3;
        switch (fan1) {
          case 0:
            options3[num] = F("Toggle Fan 1: Off");
            break;
          case 1:
            options3[num] = F("Toggle Fan 1: On");
            break;
        }
        options[num] = options3[num];
      }
      break;
    case 2: {
        dataShift(5);
        fan2 = dataRead(5);
        num = numOptions3 - 2;
        options[num] = options3[num];
        switch (fan2) {
          case 0:
            options3[num] = F("Toggle Fan 2: Off");
            break;
          case 1:
            options3[num] = F("Toggle Fan 2: On");
            break;
        }
      }
      break;
  }
}

void togglePTC() {
  dataShift(6);
  //update ptc toggle
  short num = numOptions3 - 1;
  bool b = dataRead(6);
  switch (b) {
    case 0:
      options3[num] = "Toggle PTC: Off";
      ptc = false;
      break;
    case 1:
      options3[num] = "Toggle PTC: On";
      ptc = true;
      break;
  }
  options[num] = options3[num];
}

void motorChange() {
  bool toggle = true;
  int test = 0;
  updateLCD();
  line1 = F("");
  line2 = F("  L or R to Adjust  ");
  line3 = F("");
  line4 = F("");
  lcdClear();
  dataOff(6);
  while (toggle) {
    buttonCheck();

    if (buttonRelease) {
      if (pbulk == 10) {
        toggle = false;
      }
      line2 = F("  L or R to Adjust  ");
      pbulk = 0;
      test = 0;
      dataOff(2);
      dataOff(3);
    }
    switch (pbulk) {
      case 2: {
          line2 = F("      RIGHT ->      ");
          if (test != pbulk and !door) {
            dataOn(2);
            test = pbulk;
          }
        }
        break;
      case 1: {
          line2 = F("      <- LEFT     ");
          if (test != pbulk and !door) {
            dataOn(3);
            test = pbulk;
          }
        }
        break;

    }
    line4 = F("  Select to Return");
    line1 = F(" <Motor Adustment>  ");
    updateLCD();
  }
  dataOn(6);
}
unsigned long timer1 = 0, timer2 = 0, timer3 = 0;
bool exiting = false;
bool lower = true;
short l = 0;
int count = 0;
short startUpTime = 10;
void mainStart() {
  switch (stage) {
    case 0: { //Stage 0 Setup
        line1 = F("      Stage 0       ");
        line2 = F("     Setting Up     ");
        line3 = F("                    ");
        line4 = F("                    ");

        temp_over = false;
        if (fan1) {
          toggleFan(1);
        }
        if (fan2) {
          toggleFan(2);
        }
        if (ptc) {
          togglePTC();
        }

        dataOn(1);
        digitalWrite(LA_IN_1, LOW);

        temp = pwm;
        exiting = false;
        lower = true;
        l = 0;

        updateLCD();
        delay(6500);
        timer1 =  millis();
        timer2 = 0;
        line4 = F("   Initialzing...   ");
        updateLCD();
        delay(6500);
        stage = 1;
        dataOff(1);

        //REMOVE
        //timer2 = millis();
        //timer3 = millis();
        //REMOVE
      }
      break;
    case 1: { //Stage 1
        updateLoadSensor();
        updateLoadSensor();
        int timePassed1 = int((millis() - timer1) / 1000) - startUpTime;
        int timePassed2 = int((millis() - timer2) / 1000);
        line1 = F("      Stage 1       ");
        if (timePassed1 < 0) {
          line2 = F("  Counting Down...  ");
          line3 = "   Starting in " + String(abs(timePassed1)) + "s ";
        }
        else {
          line2 = F("   Pre-Compacting   ");
          line3 = "  t1: " + String(timePassed1) + "s " + "t2: " + String(timePassed2);
        }
        line4 = "   Force: " + String(force);

        //Extend for X time
        if (exiting) {
          digitalWrite(LA_IN_1, LOW);
          dataOn(1);
          line1 = F("*      ERROR!      *");
          line2 = F("* Actuator Failure *");
          line3 = F("*  Fix Allignment  *");
          line4 = F("*     Exiting...   *");

          stage = 0;
          dataOff(6);
          updateLCD();
          delay(1000);
          updateLCD();
          delay(12000);
          mode = 0;
          modeChange();

        } else {
          if (timePassed1 >= 0) {
            if (force < 0.5) {
              l = 125;
            }
            if (force > 1) {
              l = 85;
            }
            if (force > 5) {
              l = 65;
            }
            if (force > 8) {
              l = 45;
            }
            if (force > 1 and timePassed1 < 24) {
              exiting = true;
            }
            if (door) {
              l = 0;
            }
            if (force > 15 and timePassed1 > 24 and lower) {
              lower = false;
              timer2 = millis();
              timePassed2 = int((millis() - timer2) / 1000);

            }
            if (lower and !exiting) {
              analogWrite(LA_IN_1, l);
              dataOff(1);
            } else {
              if (timer2 == millis() and !exiting) {
                digitalWrite(LA_IN_1, LOW);
                dataOn(1);
              }
              if (timePassed2 > 32 and !door and !exiting) {
                stage = 2;
                timer2 = millis();
                timer3 = millis();
                timePassed2 = int((millis() - timer2) / 1000);
                lower = true;
                temp_over = false;
                dataOff(1);
              }
            }
          }//if time passed > 0
        }//else
      }
      break;
    case 2: { //Stage 2

        int timePassed1 = int((millis() - timer1) / 1000) - startUpTime;
        int timePassed2 = int((millis() - timer2) / 1000);
        int timePassed3 = int((millis() - timer3) / 1000);
        updateLoadSensor();
        updateLoadSensor();
        updateTempSensor();
        line1 = F("       Stage 2      ");
        line2 = "HB:" + String(round(tempc1)) + "|PTC:" + String(round(tempc2)) + "|Set:" + setTemp;
        line3 = "t1:" + String(timePassed1) + " t2:" + String(timePassed2) + " t3:" + String(timePassed3);
        line4 = "Force:" + String(force) + " PTC:" + ptc;

        //TEMP
        //Check if temp oven if so, disable heating band and update status
        if (tempc1 >= setTemp and !temp_over) {
          temp = 0;
          temp_over = true;
          exiting = true;
        }

        //check status and temp over
        //if now under update status
        if (tempc1 < setTemp and temp_over) {
          temp_over = false;
          temp = pwm;
        }
        if (timePassed3 < 25) {
          analogWrite(heating_band_SSR, temp);
        } else {
          digitalWrite(heating_band_SSR, 0);
        }

        //FORCE
        if (lower) {
          if (force < 0.1) {
            l = 125;
          }
          if (force > 1) {
            l = 75;
          }
          if (force > 5) {
            l = 65;
          }
          if (force > 10) {
            l = 50;
          }
          if (door) {
            l = 0;
          }
          analogWrite(LA_IN_1, l);
          if (timePassed2 >= 24) {
            lower = false;
          }
        }

        //hold timer 3 while under 24 sec or while under setTemp
        if (timePassed2 < 24 or !exiting) {
          timer3 = millis();
          timePassed3 = int((millis() - timer3) / 1000);
        }
        if (!ptc) {
          togglePTC();
        }

        if (lower == false) {
          digitalWrite(LA_IN_1, LOW);

          if (timePassed3 >= 25) {
            stage = 3;
            timer2 = millis();
            timer3 = millis();
            timePassed2 = int((millis() - timer2) / 1000);
            timePassed3 = int((millis() - timer3) / 1000);
            exiting = false;
            lower = true;
          }
        }
      }
      break;
    case 3: { //Stage 3

        int timePassed1 = int((millis() - timer1) / 1000) - startUpTime;
        int timePassed2 = int((millis() - timer2) / 1000);
        int timePassed3 = int((millis() - timer3) / 1000);
        updateLoadSensor();
        updateLoadSensor();
        updateTempSensor();
        line1 = F("       Stage 3      ");
        line2 = "HB:" + String(round(tempc1)) + "|PTC:" + String(round(tempc2)) + "|Set:" + setTemp;
        line3 = "t1:" + String(timePassed1) + "s t2:" + String(timePassed2) + "s t3:" + String(timePassed3);
        line4 = "Force: " + String(force) + " c:" + count;

        if (lower) {
          if (force < 0.5) {
            l = 125;
          }
          if (force > 1) {
            l = 85;
          }
          if (force > 5) {
            l = 65;
          }
          if (force > 8) {
            l = 45;
          }
          if (door) {
            l = 0;
          }
          analogWrite(LA_IN_1, l);

          if (force > 10) {
            lower = false;
            digitalWrite(LA_IN_1, LOW);
            dataOn(1);
          }

          timer2 = millis();
          timer3 = millis();
        }

        if (!lower) {
          if (count < 3) {
            if (timePassed2 > 2) {
              lower = true;
              count++;
              dataOff(1);
            }
          } else {
            if (timePassed2 > 24) {
              //lower = true;
              count++;
              dataOff(1);
            }
          }
          if (count >= 4) {
            stage = 4;
            timer2 = millis();
            timer3 = millis();
            timePassed2 = int((millis() - timer2) / 1000);
            timePassed3 = int((millis() - timer3) / 1000);
            lower = true;
          }
        }
      }
      break;
    case 4: {
        int timePassed1 = int((millis() - timer1) / 1000) - startUpTime;
        int timePassed2 = int((millis() - timer2) / 1000);
        int timePassed3 = int((millis() - timer3) / 1000);
        line1 = F("       Stage 4      ");
        line2 = "HB:" + String(round(tempc1)) + "|PTC:" + String(round(tempc2)) + "|Set:" + setTemp;
        line3 = "t1:" + String(timePassed1) + "s t2:" + String(timePassed2) + "s t3:" + String(timePassed3);
        line4 = "   Force: " + String(force);
        updateLoadSensor();
        updateLoadSensor();
        updateTempSensor();
        if (timePassed2 < 9  and lower) {
          dataOn(2);
          timer3 = millis();
        } else {
          if (timePassed2 == 9) {
            dataOff(2);
          }
        }
        if (timePassed2 >= 9) {
          if (lower) {
            if (force <= 10) {
              if (force < 0.5) {
                if (timePassed2 < 24) {
                  l = 255;
                } else {
                  l = 65;
                }
              }
              if (force > 1) {
                l = 55;
              }
              if (force > 5) {
                l = 45;
              }
              if (force > 8) {
                l = 30;
              }
              if (door) {
                l = 0;
              }
              analogWrite(LA_IN_1, l);
              timer3 = millis();
            } else {
              lower = false;
            }
          }//lower
          if (timePassed3 < 3 and !lower) {
            digitalWrite(LA_IN_1, 0);
            dataOn(1);
          } else {
            if (!lower and timePassed3 == 3) {
              dataOff(1);
            }
          }
        }//Time > 9 and <
      }
      break;
  }

}
