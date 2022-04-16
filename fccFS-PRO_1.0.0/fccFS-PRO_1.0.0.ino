#include <Wire.h>
#include <LiquidCrystal_I2C_Menu.h>
LiquidCrystal_I2C_Menu lcd(0x3F, 16, 2);
#include <EEPROM.h>
#include <Keypad.h>
#include <EEPROMTyped.h>

#define pinCLK 2
#define pinDT  4
#define pinSW  3

#define stepPin 13
#define dirPin 12
#define enable 17
#define msg1 16
#define msg2 15
#define msg3 14

#define releTrig 11

// SHOW BOOTSCREEN
char * messagePadded = "                WWW.FlyCamCzech.CZ/FocusStacking                ";
void showLetters(int printStart, int startLetter)
{
  lcd.setCursor(printStart, 1);
  for (int letter = startLetter; letter <= startLetter + 15; letter++) // Print only 16 chars in Line #2 starting 'startLetter'
  {
    lcd.print(messagePadded[letter]);
  }
  lcd.print(" ");
  delay(300);
}

//------------------------------------------------------
// KEYBOARD
const byte ROWS = 1;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1', '2', '3', '4'}
};

byte rowPins[ROWS] = {5}; //row pinouts of the keypad
byte colPins[COLS] = {7, 6, 9, 8}; //column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );;

//------------------------------------------------------
// VARIABLES
int DOF;
int stepsI;
int newStepsI;
int timing;
int delayTiming;
int actualState = 0;
int motorState = 0;
int focusMotorState;
int focusCounter;

bool setStep1;
bool setStep2;
bool setStep3;
//------------------------------------------------------

// MENU ENUM
enum {menuBack,
      menuRoot,
      menuShot,
      menuSettings,
      menuDOF,
      menuTime,
      menuStep,
      menuSave,
      menuSaveApproved,
      menuRestore,
      menuRestoreApproved,
      menuDefaults,
      menuRun,
      menuRunF,
      menuRunB,
      menuDelete,
      menuGo,
      menuGoF,
      menuGoB
     };

//------------------------------------------------------
// MENU VOIDS
void SetDOF();
void msgs();
void SetDelay();
void SetStepValue();
void SetDefaults();
void SetSave();
void goRunF();
void goRunB();
void delTrace();
void goF();
void goB();
void capture();

//------------------------------------------------------
//------------------------------------------------------
//------------------------------------------------------
// VOIDS  VOIDS  VOIDS  VOIDS  VOIDS  VOIDS  VOIDS  VOIDS
void releON() {
  digitalWrite(releTrig, LOW);
}
void releOFF() {
  digitalWrite(releTrig, HIGH);
}

void capture() {
  releON();
  delay(400);
  releOFF();
  delay(450);
}

//------------------------------------------------------
// STEPPING
void setStepping() {
  digitalWrite(msg1, setStep1);
  digitalWrite(msg2, setStep2);
  digitalWrite(msg3, setStep3);
}

//------------------------------------------------------
// GO FRONT
void goFront() {
  digitalWrite(enable, LOW);
  digitalWrite(msg1, setStep1);
  digitalWrite(msg2, setStep2);
  digitalWrite(msg3, setStep3);

  digitalWrite(dirPin, HIGH);
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(1400);
  digitalWrite(stepPin, LOW);
  delayMicroseconds(1400);
  actualState++;
}

//------------------------------------------------------
// GO BACK
void goBack() {
  digitalWrite(enable, LOW);
  digitalWrite(msg1, setStep1);
  digitalWrite(msg2, setStep2);
  digitalWrite(msg3, setStep3);

  digitalWrite(dirPin, LOW);
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(1400);
  digitalWrite(stepPin, LOW);
  delayMicroseconds(1400);
}

//------------------------------------------------------
// MOTOR STOP
void motorStop() {
  digitalWrite(enable, LOW);
  digitalWrite(msg1, LOW);
  digitalWrite(msg2, LOW);
  digitalWrite(msg3, LOW);

  digitalWrite(dirPin, LOW);
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(1400);
  digitalWrite(stepPin, LOW);
  delayMicroseconds(1400);
}

//------------------------------------------------------
// GO FRONT
void goF() {
  for (int i = 0; i < motorState; i++) {
    goFront();
  }
  motorStop();
}

//------------------------------------------------------
// GO BACK
void goB() {
  for (int i = 0; i < motorState; i++) {
    goBack();
  }
  motorStop();
}

//------------------------------------------------------

// RUN BACK
void goRunB() {
  lcd.clear();
  delayTiming = timing * 1000;
  focusMotorState = (motorState / DOF);

  for (int i = 0; i < focusMotorState; i++) {
    for (int i = 0; i < DOF; i++) {
      char customKey = keypad.getKey();

      if (digitalRead(3) == LOW ) {
        break;
        LCDRepaint();
        delay(30);
      }
      goBack();
      focusCounter++;
    }
    lcd.setCursor(0, 0);
    lcd.print(String(focusCounter) + String(" from ") + String(motorState));

    char customKey = keypad.getKey();
    delay(300);
    releON();
    delay(400);
    releOFF();
    delay(100);
    delay(delayTiming);

    if (digitalRead(3) == LOW ) {
      break;
      LCDRepaint();
      delay(30);
    }
  }
  focusCounter = 0;
  focusMotorState = 0;
}

// RUN FRONT
void goRunF() {
  lcd.clear();
  delayTiming = timing * 1000;
  focusMotorState = (motorState / DOF);
  for (int i = 0; i < motorState; i++) {
    goBack();
  }

  delay(2400); // wait before changing dir

  for (int i = 0; i < focusMotorState; i++) {
    for (int i = 0; i < DOF; i++) {
      char customKey = keypad.getKey();

      if (digitalRead(3) == LOW ) {
        break;
        LCDRepaint();
        delay(30);
      }
      goFront();
      focusCounter++;
    }
    lcd.setCursor(0, 0);
    lcd.print(String(focusCounter) + String(" from ") + String(motorState));

    char customKey = keypad.getKey();
    delay(300);
    releON();
    delay(400);
    releOFF();
    delay(100);
    delay(delayTiming);

    if (digitalRead(3) == LOW ) {
      break;
      LCDRepaint();
      delay(30);
    }
  }
  focusMotorState = 0;
  focusCounter = 0;
}
//------------------------------------------------------
//......................................................
// MENU
sMenuItem menu[] = {
  // {Parent, Key, Caption, Funcion}
  {menuBack, menuRoot, "", NULL},
  {menuBack, menuRoot, "Modes", NULL},

  {menuRoot, menuShot, "CAPTURE", capture},

  {menuRoot, menuRun, "RUN", NULL},
  {menuRun, menuRunF, "RUN Front", goRunF},
  {menuRun, menuRunB, "RUN Back", goRunB},
  {menuRun, menuDelete, "Delete TRACE", delTrace},
  {menuRun, menuBack, "EXIT", NULL},

  {menuRoot, menuGo, "GO", NULL},
  {menuGo, menuGoB, "GO Back", goB},
  {menuGo, menuGoF, "GO Front", goF},
  {menuGo, menuDelete, "Delete TRACE", delTrace},
  {menuGo, menuBack, "EXIT", NULL},

  {menuRoot, menuSettings, "SETTINGS", NULL},
  {menuSettings, menuStep, "Step", SetStepValue},
  {menuSettings, menuDOF, "DoF", SetDOF},
  {menuSettings, menuTime, "Time", SetDelay},
  {menuSettings, menuBack, "EXIT", NULL},

  {menuRoot, menuSave, "SAVE", NULL},
  {menuSave, menuSaveApproved, "Save to EEPROM", SetSave},
  {menuSave, menuBack, "EXIT", NULL},

  {menuRoot, menuRestore, "RESTORE  ", NULL},
  {menuRestore, menuRestoreApproved, "Restore EEPROM", SetDefaults},
  {menuRestore, menuBack, "EXIT", NULL},

  {menuRoot, menuBack, "EXIT", NULL},
};
uint8_t menuLen = sizeof(menu) / sizeof(sMenuItem);

int getItemIndexByKey(uint8_t key) {
  for (uint8_t i = 0; i < menuLen; i++) {
    if (menu[i].key == key) {
      return i;
    }
    return -1;
  }
}

//------------------------------------------------------
//......................................................
void LCDRepaint() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(String("DoF ") + String(DOF));
  lcd.setCursor(7, 0);
  switch (stepsI) {
    case 1:
      lcd.print(String("Step ") + String("1/1"));
      break;

    case 2:
      lcd.print(String("Step ") + String("1/2"));
      break;

    case 3:
      lcd.print(String("Step ") + String("1/4"));
      break;

    case 4:
      lcd.print(String("Step ") + String("1/8"));
      break;

    case 5:
      lcd.print(String("Step ") + String("1/16"));
      break;

    default:
      lcd.print(String("Step ") + String("1/32"));
      break;
  }
  lcd.setCursor(0, 1);
  lcd.print(String("Delay ") + String(timing) + String("s"));
}

//------------------------------------------------------
//......................................................
void delTrace() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("TRACE deleted");
  actualState = 0;
  motorState = 0;
  delay(2000);
  LCDRepaint();
}

//------------------------------------------------------
//......................................................
void SetStepValue() {
  lcd.setCursor(0, 0);
  lcd.print("1=1/1  6=1/32");
  delay(2500);
  stepsI = lcd.inputVal<int>("Input Steps", 1, 6, stepsI, 1);

}

//------------------------------------------------------
void SetDOF() {
  lcd.setCursor(0, 0);
  lcd.print("1=0,1 mm 16=1 mm");
  delay(2500);
  DOF = lcd.inputVal<int>("Input DoF", 1, 16, DOF, 2);
}

//------------------------------------------------------
void SetDelay() {
  timing = lcd.inputVal<int>("Input delay (s)", 1, 16, timing);
}

//------------------------------------------------------
void SetSave() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("EEPROM SAVED");
  EEPROM.update(1, timing);
  EEPROM.update(2, stepsI);
  EEPROM.update(3, DOF);
  delay(1000);
}

//------------------------------------------------------
void SetDefaults() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("EEPROM RESTORED");
  stepsI = 3;
  timing = 2;
  DOF = 10;
  EEPROM.update(1, timing);
  EEPROM.update(2, stepsI);
  EEPROM.update(3, DOF);
  delay(1000);
}

//------------------------------------------------------
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//------------------------------------------------------
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
void setup() {
  lcd.begin();
  lcd.attachEncoder(pinCLK, pinDT, pinSW);
  timing = EEPROM.read(1); // initialize EEPROM
  stepsI = EEPROM.read(2); // initialize EEPROM
  DOF = EEPROM.read(3);    // initialize EEPROM
  delay(200);

  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(enable, OUTPUT);
  pinMode(msg1, OUTPUT);
  pinMode(msg2, OUTPUT);
  pinMode(msg3, OUTPUT);
  pinMode (releTrig, OUTPUT);
  //------------------------------------------------
  //BOOT SCREEN
  lcd.setCursor(0, 0);
  lcd.print("fccFocusStacking");
  for (int letter = 0; letter <= strlen(messagePadded) - 16; letter++)
  {
    showLetters(0, letter);
  }
  lcd.clear();
  LCDRepaint();
  focusCounter = 1; //1 due restriction of division with 0
}

//------------------------------------------------------
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//------------------------------------------------------
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
void loop() {
  digitalWrite(enable, LOW);
  digitalWrite(msg1, LOW);
  digitalWrite(msg2, LOW);
  digitalWrite(msg3, LOW);

  char customKey = keypad.getKey();

  if (newStepsI != stepsI) {
    switch (stepsI) {
      case 1:
        setStep1 = 0;
        setStep2 = 0;
        setStep3 = 1;
        newStepsI = 1;
        break;
      case 2:
        setStep1 = 0;
        setStep2 = 1;
        setStep3 = 0;
        newStepsI = 2;
        break;

      case 3:
        setStep1 = 0;
        setStep2 = 1;
        setStep3 = 1;
        newStepsI = 3;
        break;

      case 4:
        setStep1 = 1;
        setStep2 = 0;
        setStep3 = 1;
        newStepsI = 4;
        break;

      case 5:
        setStep1 = 1;
        setStep2 = 1;
        setStep3 = 0;
        newStepsI = 5;
        break;

      default:
        setStep1 = 1;
        setStep2 = 1;
        setStep3 = 1;
        newStepsI = 6;
        break;
    }
  }

  // IF 1 PRESSED (GO BACK)
  if (customKey == '1') {
    for (;;) {
      char customKey = keypad.getKey();
      goBack();
      if (actualState == 0) { //to not have negative value
        actualState = motorState;
      }
      if (actualState >> 0) {
        actualState--;
      }
      if (customKey == '1') {
        break;
        LCDRepaint();
        delay(30);
      }
    }
  }

  // IF 2 PRESSED (SAVE BACK)
  if (customKey == '2') {
    motorState = 0;
    delay(10);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("START POSITION");
    lcd.setCursor(0, 1);
    lcd.print("SAVED");
    delay(500); //lengt of showing -- can be set at top of code
    actualState = 0;
    LCDRepaint();
  }

  // IF 3 PRESSED (GO FRONT)
  if (customKey == '3') {
    for (;;) {
      char customKey = keypad.getKey();
      goFront();
      if (customKey == '3') {
        break;
        LCDRepaint();
        delay(30);
      }
    }
  }

  // IF 4 PRESSED (SAVE FRONT)
  if (customKey == '4') {
    motorState = actualState;
    delay(10);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("END POSITION");
    lcd.setCursor(0, 1);
    lcd.print("SAVED");
    delay(500); //lengt of showing -- can be set at top of code
    LCDRepaint();
  }

  if (lcd.getEncoderState() == eButton) {
    lcd.showMenu(menu, menuLen, 1);
    LCDRepaint();
  }
}
