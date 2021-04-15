// include the library code:
#include <LiquidCrystal.h>
#include <LM35.h>
#include <HX711_ADC.h>
#if defined(ESP8266)|| defined(ESP32) || defined(AVR)
#include <EEPROM.h>
#endif

# define data 9
# define clockPin 8

const byte HX711_dout = 13; //mcu > HX711 dout pin
const byte HX711_sck = 12; //mcu > HX711 sck pin

//HX711 constructor:
HX711_ADC LoadCell(HX711_dout, HX711_sck);



// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const byte
LCD_D7 = 2, LCD_D6 = 3, LCD_D5 = 4, LCD_D4 = 5, LCD_EN = 6, LCD_RS = 7,
LA_IN_1 = 10, LA_IN_2 = 1, M_IN_1 = 13, M_IN_2 = 13, RH_IN = 13,
heating_band_SSR = 11;

const byte
button1 = A5, button2 = A4, button3 = A3,
latch = A2
        ;


//red = 8, black = 9, brown = 10
//white = 7, black = 6, brown = 5, red = 4, orange = 3, yellow = 2

LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
LM35 sensor1(0), sensor2(1);

bool
diagnostic = false,
completed, temp_over = false,
button1State, button2State, button3State,
optionsMenu = false, buttonRelease,
door,
fan1 = false, fan2 = false,
ptc = false;

float
tempc1, tempc2, temp, pwm = 43, pwm_actuator,
        debounceDelay = 50,
        force = 0;

short
setTemp = 30,
pwmOptions = 6;

unsigned long lastDebounceTime1 = 0;  // the last time the output pin was toggled
unsigned long lastDebounceTime2 = 0;  // the last time the output pin was toggled
unsigned long lastDebounceTime3 = 0;  // the last time the output pin was toggled

unsigned long hold, phold;

short
bulk, pbulk, currPos = 0,
             mode = 0,
             load = 250,
             numOptions,
             stage = 0,
             serial = 0;
;

String line1, line2, line3, line4;

byte dataOut;

const short numOptions1 = 4, numOptions2 = 2, numOptions3 = 10, numOptions4 = 4, numOptions5 = 3;
String options[10];
String options1[numOptions1] = {
  "Start",
  "Options",
  "Info",
  "Diagnostics"
};
//START - MODE 1
String options2[numOptions2] = {
  "Info",
  "Stop"
};
//DIAGNOSTICS - MODE 2
String options3[numOptions3] = {
  "Back",
  "Set Max Temp",
  "Set Band PWM",
  "Set Actuator PWM",
  "Test Motor",
  "Adjust Piston",
  "AutoHeat Mode",
  "Toggle Fan 1: Off",
  "Toggle Fan 2: Off",
  "Toggle PTC: Off"
};

//OPTIONS - MODE 3
String options4[numOptions4] = {
  "Back",
  "Change Heat Mode",
  "Change PWM",
  "Adjust Piston"
};

//Confirm SubMenu
String options5[numOptions5] = {
  "Confirm?",
  "Yes",
  "No"
};

void setup() {
  lcd.begin(20, 4);
  lcdPrint("Densify BETA Build", 0, 1);
  delay(load);
  lcdPrint("Loading", 0, 0);
  delay(load);
  lcdPrint("Loading.", 0, 0);
  delay(load);
  lcdPrint("Loading..", 0, 0);
  lcdPrint(" Kishon|Brian|Stan", 0, 2);
  lcdPrint("     Jada|Harman    ", 0, 3);
  delay(load);
  lcdPrint("Loading...   =]", 0, 0);

  byte base = B00000100;
  dataOut = base;
  pinMode(clockPin, OUTPUT); // make the clock pin an output
  pinMode(data , OUTPUT); // make the data pin an output
  shiftOut(data, clockPin, LSBFIRST, base); // send this binary value to the shift register

  pinMode(button1, INPUT_PULLUP); pinMode(button2, INPUT_PULLUP); pinMode(button3, INPUT_PULLUP);
  pinMode(heating_band_SSR, OUTPUT);
  pinMode(LA_IN_1, OUTPUT);
  pinMode(LA_IN_2, OUTPUT);
  //pinMode(RH_IN, OUTPUT);
  //digitalWrite(RH_IN, 1);
  startLoadSensor();
  updateLoadSensor();
  attachInterrupt(digitalPinToInterrupt(HX711_dout), dataReadyISR, FALLING);

  modeChange();
  digitalWrite(LA_IN_1, LOW);
  dataOn(1);
}
volatile bool newDataReady;
void dataReadyISR() {
  if (LoadCell.update()) {
    newDataReady = 1;
  }
}

void(* hardReset)  (void) = 0;
void softReset() {

}

unsigned long t;
void updateLoadSensor() {
  //LoadCell.powerUp();
  if (LoadCell.update()) {
    if (millis() > t + 65) {
      force = LoadCell.getData();
      t = millis();
    }
  }

}

void startLoadSensor() {
  LoadCell.begin();
  LoadCell.start(300, true);
  LoadCell.setCalFactor(-9759.3183);
}

unsigned long s;
void updateTempSensor() {
  if (millis() > s + 200) {
    tempc1 = sensor1.getTemp(CELCIUS);
    tempc2 = sensor2.getTemp(CELCIUS);
    s = millis();
  }
}


void loop() {
  //process checks and updates
  buttonCheck();
  buttonUpdate();


  //detect current mode
  if (!optionsMenu) {
    switch (mode) {
      //HOME SCREEN
      case 0:
        currPos = 0;
        line1 = F("|   Densify v1.0   |");
        line2 = F("|                  |");
        line3 = F("|   Press < or >   |");
        line4 = F("|   Build: Beta    |");
        break;

      //START
      case 1:
        mainStart();
        updateLCD();
        break;

      //DIAGNOSTICS
      case 2:
        optionsMenu = true;
        currPos = 1;
        menuUpdate();
        break;
      //INFO
      case 3:
        optionsMenu = true;
        currPos = 1;
        menuUpdate();
        break;
      case 4:
        optionsMenu = true;
        currPos = 1;
        menuUpdate();
        break;
    }

  }
  updateLCD();
}//Void Loop
