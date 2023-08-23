#include "fccFS-PRO_1.1.6a.h"
#include <Arduino.h>

#include <Wire.h>
#include <LiquidCrystal_I2C_Menu.h>

LiquidCrystal_I2C_Menu lcd(0x27, 16, 2);  //0x27order / 3f me
#include <EEPROM.h>
#include <Keypad.h>
#include <EEPROMTyped.h>

//------------------------------------------------------
//------------------------------------------------------
// user defines
#define wait 500
//------------------------------------------------------
//------------------------------------------------------

#define pinCLK 2
#define pinDT 4
#define pinSW 3
#define stepPin 13
#define dirPin 12
#define enable 17
#define msg1 16
#define msg2 15
#define msg3 14
#define releTrig 11

//------------------------------------------------------
// EEPROM function
void writeIntIntoEEPROM(int address, int number) {
  EEPROM.write(address, number >> 8);
  EEPROM.write(address + 1, number & 0xFF);
}
int readIntFromEEPROM(int address) {
  return (EEPROM.read(address) << 8) + EEPROM.read(address + 1);
}
//------------------------------------------------------
// SHOW BOOTSCREEN
char* messagePadded = "     v1.1.6a    www.FlyCamCzech.cz/FocusStacking                ";  //pointer
void showLetters(int printStart, int startLetter) {
  lcd.setCursor(printStart, 1);
  for (int letter = startLetter; letter <= startLetter + 15; letter++)  // Print only 16 chars in Line #2 starting 'startLetter'
  {
    lcd.print(messagePadded[letter]);
  }
  lcd.print(" ");
  delay(350);
}

//------------------------------------------------------
// KEYBOARD
const byte ROWS = 1;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  { '4', '3', '2', '1' }  //1,4,2,3 order    1,2,3,4 mine   4,3,2,1 Fero
};
byte rowPins[ROWS] = { 5 };           //row pinouts of the keypad
byte colPins[COLS] = { 7, 6, 9, 8 };  //column pinouts of the keypad
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

//------------------------------------------------------
// VARIABLES
int DOF;
unsigned int localDOF = 0;
int localAdded;
int percentage;
int stepsI;
int ShootMode;
int AutoReturn;
int newStepsI;
int timing;
int timingMove;
int motorSpeed;
int motorDelay;
int added;
int delayTiming;
int delayTimingMove;
int delayTimingTrigger;
int trigger;
int mirror;
int bootVal;
int presetVal;
int autoSave;
unsigned long actualState = 0;
unsigned long motorState = 0;
unsigned long focusMotorState = 0;
unsigned long focusCounter = 0;
unsigned long loopCounter = 0;
int emergency = 0;
bool setStep1;
bool setStep2;
bool setStep3;

int firstRun = 0;
//------------------------------------------------------
// MENU ENUM
enum { menuBack,
       menuRoot,
       menuShot,
       menuStep,
       menuSettings,
       menuPreferences,
       menuDOF,
       menuTime,
       menuTimeMove,
       menuTimeTrigger,
       menuShootMode,
       menuMirror,
       menuAutoReturn,
       menuSpeed,
       menuAdded,
       menuSave_Restore,
       menuSave,
       menuSaveFocusStack,
       menuSaveMicroscope,
       menuRestore,
       menuRestoreFocusStack,
       menuRestoreMicroscope,
       menuDefaults,
       menuMode,
       menuModeM,
       menuModeFS,
       menuBoot,
       menuPreset,
       menuPresetM,
       menuPresetR,
       menuInfo,
       menuInfoDisp,
       menuRun,
       menuRunF,
       menuRunB,
       menuRunFsettings,
       menuRunBsettings,
       menuDelete,
       menuGo,
       menuGoF,
       menuGoB,
       menuAutoSave,
     };
//------------------------------------------------------
// MENU VOIDS
void SetDOF();
void msgs();
void SetDelay();
void SetDelayMove();
void SetDelayTrigger();
void SetShootMode();
void SetStepValue();
void SetAutoReturn();
void SetMirror();
void SetAdded();
void SetDefaultsFS();
void SetDefaultsM();
void SetSpeed();
void SetSave();
void goRunFsettings();
void goRunBsettings();
void goRunF();
void goRunB();
void delTrace();
void goF();
void goB();
void capture();
void SetBoot();
void SetModeFS();
void SetModeM();
void SetPresetR();
void SetPresetM();
void SetAutoSave();
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

//------------------------------------------------------
void capture() {
  if (ShootMode == 0) {
    releON();
    delay(trigger);
    releOFF();
    delay(trigger);
  }

  if (ShootMode == 1) {
    releON();
    delay(trigger);
    releOFF();
    delay(mirror);
    releON();
    delay(trigger);
    releOFF();
  }
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
void goBack() {
  digitalWrite(enable, LOW);
  digitalWrite(msg1, setStep1);
  digitalWrite(msg2, setStep2);
  digitalWrite(msg3, setStep3);

  digitalWrite(dirPin, HIGH);
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(motorDelay);
  digitalWrite(stepPin, LOW);
  delayMicroseconds(motorDelay);
  actualState++;
}

void goBack2() {
  digitalWrite(enable, LOW);
  digitalWrite(msg1, setStep1);
  digitalWrite(msg2, setStep2);
  digitalWrite(msg3, setStep3);

  digitalWrite(dirPin, HIGH);
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(900);
  digitalWrite(stepPin, LOW);
  delayMicroseconds(900);
}

//------------------------------------------------------
// GO BACK
void goFront() {
  digitalWrite(enable, LOW);
  digitalWrite(msg1, setStep1);
  digitalWrite(msg2, setStep2);
  digitalWrite(msg3, setStep3);

  digitalWrite(dirPin, LOW);
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(motorDelay);
  digitalWrite(stepPin, LOW);
  delayMicroseconds(motorDelay);
}

void goFront2() {
  digitalWrite(enable, LOW);
  digitalWrite(msg1, setStep1);
  digitalWrite(msg2, setStep2);
  digitalWrite(msg3, setStep3);

  digitalWrite(dirPin, LOW);
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(900);
  digitalWrite(stepPin, LOW);
  delayMicroseconds(900);
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
  delayMicroseconds(motorDelay);
  digitalWrite(stepPin, LOW);
  delayMicroseconds(motorDelay);
}

//------------------------------------------------------
// GO Back
void goB() {
  for (int i = 0; i < motorState; i++) {
    goBack();
  }
  motorStop();
}

//------------------------------------------------------
// GO Front
void goF() {
  for (int i = 0; i < motorState; i++) {
    goFront();
  }
  motorStop();
}

//------------------------------------------------------
// RUN front
void goRunF() {
  lcd.clear();

  if (DOF >= 20 and DOF <= 60) {
    motorDelay = 80;
  }
  else if (DOF <= 19) {
    motorDelay = 10;
  }
  else motorDelay = motorSpeed;

  loopCounter = 0;
  focusCounter = 0;
  emergency = 0;

  switch (presetVal) {
    case 0:
      switch (stepsI) {
        case 0:
          localDOF = ((DOF / 2) * 0.75);
          focusMotorState = (motorState / localDOF);
          break;

        case 1:
          localDOF = (DOF * 0.75);
          focusMotorState = (motorState / localDOF);
          break;
      }
      break;

    case 1:
      switch (stepsI) {
        case 0:
          localDOF = ((DOF / 2) * 0.8);
          focusMotorState = (motorState / localDOF);
          break;

        case 1:
          localDOF = (DOF * 0.8);
          focusMotorState = (motorState / localDOF);
          break;
      }
      break;
  }

  for (int i = 0; i < motorState; i++) {
    goBack2();
  }

  delayTiming = timing * 1000;
  delayTimingMove = timingMove * 1000;
  delay(2400);  // wait before changing dir

  for (int i = 0; i < focusMotorState; i++) {
    loopCounter++;
    for (int i = 0; i < localDOF; i++) {
      char customKey = keypad.getKey();

      if (digitalRead(3) == LOW) {
        emergency = 1;
        break;
        LCDRepaint();
        delay(30);
      }
      goFront();
      focusCounter++;
    }

    lcd.clear();
    int remainingTime = (((trigger * 2) + mirror + 5000 + delayTiming + delayTimingMove) / 2000);
    int laps = (focusMotorState - loopCounter);

    lcd.setCursor(0, 0);
    lcd.print(String(loopCounter) + String("/") + String(focusMotorState));

    lcd.setCursor(10, 0);
    lcd.print(String(laps * remainingTime) + String("s"));

    lcd.setCursor(0, 1);
    lcd.print(String("increment "));

    lcd.setCursor(10, 1);
    switch (presetVal) {
      case 0:
        lcd.print(String(DOF) + String("nm"));
        break;

      case 1:
        lcd.print(String(DOF) + String("um"));
        break;
    }

    delay(delayTimingMove);
    char customKey = keypad.getKey();
    capture();
    delay(delayTiming);
    if (digitalRead(3) == LOW) {
      emergency = 1;
      break;
      LCDRepaint();
      delay(30);
      motorStop();
    }
  }

  emergency = 0;
  focusCounter = 0;
  focusMotorState = 0;
  motorStop();
}

//------------------------------------------------------

// RUN Back
void goRunB() {
  lcd.clear();
  if (DOF >= 50) {
    motorDelay = 800;
  }
  else {
    motorDelay = 300;
  }
  loopCounter = 0;
  focusCounter = 0;
  emergency = 0;


  switch (presetVal) {
    case 0:
      switch (stepsI) {
        case 0:
          localDOF = ((DOF / 2) * 0.75);
          focusMotorState = (motorState / localDOF);
          break;

        case 1:
          localDOF = (DOF * 0.75);
          focusMotorState = (motorState / localDOF);
          break;
      }
      break;

    case 1:
      switch (stepsI) {
        case 0:
          localDOF = ((DOF / 2) * 0.8);
          focusMotorState = (motorState / localDOF);
          break;

        case 1:
          localDOF = (DOF * 0.8);
          focusMotorState = (motorState / localDOF);
          break;
      }
      break;
  }

  delayTiming = timing * 1000;
  delayTimingMove = timingMove * 1000;
  delay(2400);  // wait before changing dir

  for (int i = 0; i < focusMotorState; i++) {
    loopCounter++;

    for (int i = 0; i < localDOF; i++) {
      char customKey = keypad.getKey();

      if (digitalRead(3) == LOW) {
        emergency = 1;
        break;
        LCDRepaint();
        delay(30);
      }
      goBack();
      focusCounter++;
    }
    lcd.clear();
    int remainingTime = (((trigger * 2) + mirror + 5000 + delayTiming + delayTimingMove) / 2000);
    int laps = (focusMotorState - loopCounter);
    lcd.setCursor(0, 0);
    lcd.print(String(loopCounter) + String("/") + String(focusMotorState));

    lcd.setCursor(10, 0);
    lcd.print(String(laps * remainingTime) + String("s"));

    lcd.setCursor(0, 1);
    lcd.print(String("increment "));

    lcd.setCursor(10, 1);
    switch (presetVal) {
      case 0:
        lcd.print(String(DOF) + String("nm"));
        break;

      case 1:
        lcd.print(String(DOF) + String("um"));
        break;
    }

    delay(delayTimingMove);
    char customKey = keypad.getKey();
    capture();
    delay(delayTiming);

    if (digitalRead(3) == LOW) {
      emergency = 1;
      break;
      LCDRepaint();
      delay(30);
      motorStop();
    }
  }

  if ((AutoReturn == 1) and (emergency == 0)) {
    for (int i = 0; i < motorState; i++) {
      goFront();
    }
    motorStop();
  }

  emergency = 0;
  focusMotorState = 0;
  focusCounter = 0;
  motorStop();
}

/*
  void goRunB() {
  lcd.clear();

  if (DOF >= 20 and DOF <= 60) {
    motorDelay = 80;
  }
  else if (DOF <= 19) {
    motorDelay = 10;
  }
  else motorDelay = motorSpeed;

  loopCounter = 0;
  focusCounter = 0;
  emergency = 0;

  switch (presetVal) {
    case 0:
      switch (stepsI) {
        case 0:
          localDOF = DOF;
          focusMotorState = motorState / localDOF;
          break;

        case 1:
          localDOF = DOF;
          focusMotorState = motorState / localDOF;
          break;
      }
      break;

    case 1:
      switch (stepsI) {
        case 0:
           localDOF = DOF;
          focusMotorState = motorState / localDOF;
          break;

        case 1:
          localDOF = DOF;
          focusMotorState = motorState / localDOF;
          break;
      }
      break;
  }

  delayTiming = timing * 1000;
  delayTimingMove = timingMove * 1000;
  focusMotorState = (motorState / localDOF);
  delay(2400);  // wait before changing dir

  for (int i = 0; i < focusMotorState; i++) {
    loopCounter++;

    for (int i = 0; i < localDOF; i++) {
      char customKey = keypad.getKey();

      if (digitalRead(3) == LOW) {
        emergency = 1;
        break;
        LCDRepaint();
        delay(30);
      }
      goBack();
      focusCounter++;
    }
    lcd.clear();
    int remainingTime = (((trigger * 2) + mirror + 5000 + delayTiming + delayTimingMove) / 2000);
    int laps = (focusMotorState - loopCounter);
    lcd.setCursor(0, 0);
    lcd.print(String(loopCounter) + String("/") + String(focusMotorState));

    lcd.setCursor(10, 0);
    lcd.print(String(laps * remainingTime) + String("s"));

    lcd.setCursor(0, 1);
    lcd.print(String("increment "));

    lcd.setCursor(10, 1);
    lcd.print(String(DOF) + String("um"));

    delay(delayTimingMove);
    char customKey = keypad.getKey();
    capture();
    delay(delayTiming);

    if (digitalRead(3) == LOW) {
      emergency = 1;
      break;
      LCDRepaint();
      delay(30);
      motorStop();
    }
  }

  if ((AutoReturn == 1) and (emergency == 0)) {
    for (int i = 0; i < motorState; i++) {
      goFront();
    }
    motorStop();
  }

  emergency = 0;
  focusMotorState = 0;
  focusCounter = 0;
  motorStop();
  }
*/
//------------------------------------------------------
//......................................................
// MENU
sMenuItem menu[] = {
  // {Parent, Key, Text(caption), Funcion(handler)}
  { menuBack, menuRoot, "", NULL },
  { menuBack, menuRoot, "fccFS2 PRO", NULL },

  { menuRoot, menuShot, "testSHOT", capture },

  { menuRoot, menuRun, "RUN", NULL },
  { menuRun, menuRunFsettings, " RUN Forward", NULL },
  { menuRunFsettings, menuDOF, " um/step", SetDOF },
  { menuRunFsettings, menuRunF, " RUN", goRunF },
  { menuRunFsettings, menuBack, " goBACK", NULL },

  { menuRun, menuRunBsettings, " RUN Backward", NULL },
  { menuRunBsettings, menuDOF, " um/step", SetDOF },
  { menuRunBsettings, menuRunB, " RUN", goRunB },
  { menuRunBsettings, menuBack, " goBACK", NULL },

  { menuRun, menuDelete, " Delete TRACE", delTrace },
  { menuRun, menuBack, " EXIT", NULL },

  { menuRoot, menuGo, "GO position", NULL },
  { menuGo, menuGoB, " BACK pos.", goB },
  { menuGo, menuGoF, " FRONT pos.", goF },
  { menuGo, menuDelete, " Delete TRACE", delTrace },
  { menuGo, menuBack, " EXIT", NULL },

  { menuRoot, menuPreferences, "PREFERENCES", NULL },
  { menuPreferences, menuStep, " HiRes.", SetStepValue },
  { menuPreferences, menuDOF, " um/step", SetDOF },
  { menuPreferences, menuTime, " time shutter", SetDelay },
  { menuPreferences, menuTimeMove, " time vibr.", SetDelayMove },
  { menuPreferences, menuShootMode, " mirrorLock", SetShootMode },
  { menuPreferences, menuMirror, " time (mirror)", SetMirror },
  { menuPreferences, menuSave, " SAVE", NULL },
  { menuPreferences, menuBack, " EXIT", NULL },

  { menuRoot, menuSettings, "SETTINGS", NULL },
  { menuSettings, menuAutoReturn, " AutoReturn", SetAutoReturn },
  { menuSettings, menuTimeTrigger, " time (trigg)", SetDelayTrigger },
  { menuSettings, menuSpeed, " motor speed", SetSpeed },
  { menuSettings, menuAdded, " added length", SetAdded },
  { menuSettings, menuBoot, " BOOT screen", SetBoot },
  { menuSettings, menuPreset, " PRESET a.LOAD", NULL },
  { menuPreset, menuPresetM, " Microscope", SetPresetM },
  { menuPreset, menuPresetR, " Rail", SetPresetR },
  { menuSettings, menuAutoSave, " AutoSAVE", SetAutoSave },
  { menuPreset, menuBack, " goBACK", NULL },

  { menuSettings, menuSave, " SAVE", NULL },
  { menuSettings, menuBack, " EXIT", NULL },

  { menuRoot, menuMode, "Rail/Microscope", NULL },
  { menuMode, menuModeFS, " Rail", SetModeFS },
  { menuMode, menuModeM, " Microscope", SetModeM },
  { menuMode, menuPreset, " PRESET a.LOAD", NULL },
  { menuMode, menuBack, " EXIT", NULL },

  { menuRoot, menuSave_Restore, "SAVE/RESTORE", NULL },
  { menuSave_Restore, menuSave, " SAVE PRESET", NULL },
  { menuSave, menuSaveMicroscope, " Save", SetSave},
  //{ menuSave, menuSaveFocusStack, " FocusStack", SetSaveFS },
  //{ menuSave, menuSaveMicroscope, " Microscope", SetSaveM },
  { menuSave, menuBack, " go Back", NULL },
  { menuSave_Restore, menuRestore, " LOAD DEFAULTS  ", NULL },
  { menuRestore, menuRestoreFocusStack, " Rail", SetDefaultsFS },
  { menuRestore, menuRestoreMicroscope, " Microscope", SetDefaultsM },
  { menuRestore, menuBack, " goBACK", NULL },
  { menuSave_Restore, menuBack, " EXIT", NULL },

  { menuRoot, menuInfo, "VERSION", NULL },
  { menuInfo, menuInfoDisp, "model: fccFS2 PRO ", NULL },
  { menuInfo, menuInfoDisp, "by FlyCamCzech", NULL },
  { menuInfo, menuInfoDisp, "version 1.1.6a", NULL },
  { menuInfo, menuInfoDisp, "23. Aug. 2023", NULL },
  { menuInfo, menuBack, " EXIT", NULL },

  { menuRoot, menuBack, "EXIT", NULL },
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

  if (firstRun == 0) {
    lcd.clear();

    lcd.setCursor(0, 0);
    switch (presetVal) {
      case 0:
        lcd.print(F("Micr."));
        break;

      case 1:
        lcd.print(F("Rail"));
        break;
    }

    lcd.setCursor(6, 0);
    switch (autoSave) {
      case 0:
        lcd.print(F("a.SAVE OFF"));
        break;

      case 1:
        lcd.print(F("a.SAVE ON"));
        break;
    }

    lcd.setCursor(2, 1);
    switch (AutoReturn) {
      case 0:
        lcd.print(F("autoReturn OFF"));
        break;

      case 1:
        lcd.print(F("autoReturn ON"));
        break;
    }

    delay(4000);
    firstRun = 1;

    lcd.clear();
    lcd.setCursor(0, 0);

    switch (presetVal) {
      case 0:
        lcd.print(String("nm ") + String(DOF));
        break;

      case 1:
        lcd.print(String((char)228 + String("m ")) + String(DOF));
        break;
    }

    lcd.setCursor(8, 0);
    switch (ShootMode) {
      case 0:
        lcd.print(F("N"));
        break;

      case 1:
        lcd.print(F("L"));
        break;
    }

    lcd.setCursor(10, 0);
    switch (presetVal) {
      case 0:
        lcd.print(F("M"));
        break;

      case 1:
        lcd.print(F("R"));
        break;
    }

    lcd.setCursor(12, 0);
    switch (stepsI) {
      case 0:
        lcd.print(F("norm"));
        break;

      case 1:
        lcd.print(F("hRes"));
        break;
    }

    lcd.setCursor(0, 1);
    lcd.print(String("exp ") + String(timing) + String("s"));
    lcd.setCursor(7, 1);
    lcd.print(String(" move ") + String(timingMove) + String("s"));  //timingMove
  }
  lcd.clear();
  lcd.setCursor(0, 0);

  switch (presetVal) {
    case 0:
      lcd.print(String("nm ") + String(DOF));
      break;

    case 1:
      lcd.print(String((char)228 + String("m ")) + String(DOF));
      break;
  }

  lcd.setCursor(8, 0);
  switch (ShootMode) {
    case 0:
      lcd.print(F("N"));
      break;

    case 1:
      lcd.print(F("L"));
      break;
  }

  lcd.setCursor(10, 0);
  switch (presetVal) {
    case 0:
      lcd.print(F("M"));
      break;

    case 1:
      lcd.print(F("R"));
      break;
  }

  lcd.setCursor(12, 0);
  switch (stepsI) {
    case 0:
      lcd.print(F("norm"));
      break;

    case 1:
      lcd.print(F("hRes"));
      break;
  }

  lcd.setCursor(0, 1);
  lcd.print(String("exp ") + String(timing) + String("s"));
  lcd.setCursor(7, 1);
  lcd.print(String(" move ") + String(timingMove) + String("s"));  //timingMove
}

//------------------------------------------------------
//......................................................
void delTrace() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("TRACE deleted"));
  actualState = 0;
  motorState = 0;
  delay(2500);
  LCDRepaint();
}

//------------------------------------------------------
//......................................................
void SetAutoSave() {
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print(F("autoSAVE"));
  lcd.setCursor(2, 1);
  lcd.print(F("new values"));
  delay(1750);

  autoSave = lcd.inputVal<int>("0 = OFF / 1 = ON", 0, 1, autoSave, 1);
  writeIntIntoEEPROM(66, autoSave);
  delay(500);
}
//------------------------------------------------------

void SetSpeed() {
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print(F("MotorSpeed"));
  lcd.setCursor(0, 1);
  lcd.print(F("per single step"));
  delay(1750);

  motorSpeed = lcd.inputVal<int>("Input in ms", 200, 1000, motorSpeed, 50);

  switch (autoSave) {
    case 0:
      switch (presetVal) {
        case 0:
          delay(100);
          break;

        case 1:
          delay(100);
          break;
      }
      break;

    case 1:
      switch (presetVal) {
        case 0:
          writeIntIntoEEPROM(60, motorSpeed);
          delay(200);
          break;

        case 1:
          writeIntIntoEEPROM(27, motorSpeed);
          delay(200);
          break;
      }
      break;
  }


}

//------------------------------------------------------
void SetStepValue() {
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print(F("HiRes step"));
  lcd.setCursor(1, 1);
  lcd.print(F("movement setup"));
  delay(1750);

  stepsI = lcd.inputVal<int>("0 = OFF / 1 = ON", 0, 1, stepsI, 1);
  actualState = 0;
  motorState = 0;

  switch (autoSave) {
    case 0:
      switch (presetVal) {
        case 0:
          delay(100);
          break;

        case 1:
          delay(100);
          break;
      }
      break;

    case 1:
      switch (presetVal) {
        case 0:
          writeIntIntoEEPROM(36, stepsI);
          delay(200);
          break;

        case 1:
          writeIntIntoEEPROM(3, stepsI);
          delay(200);
          break;
      }
      break;
  }
}

//------------------------------------------------------
/*
  void SetDOF() {
  lcd.setCursor(0, 0);
  lcd.print("um/step    ");
  delay(250);
  DOF = lcd.inputVal<int>("Input um/step", 5, 1250, DOF, 5);
  }
*/


//------------------------------------------------------
void SetModeFS() {

  stepsI = readIntFromEEPROM(3);
  timing = readIntFromEEPROM(6);
  DOF = readIntFromEEPROM(9);
  timingMove = readIntFromEEPROM(12);
  trigger = readIntFromEEPROM(15);
  mirror = readIntFromEEPROM(18);
  ShootMode = readIntFromEEPROM(21);
  AutoReturn = readIntFromEEPROM(24);
  motorSpeed = readIntFromEEPROM(27);
  added = readIntFromEEPROM(30);
  bootVal = readIntFromEEPROM(33);
  autoSave = readIntFromEEPROM(66);
  delay(100);
  writeIntIntoEEPROM(3, stepsI);
  writeIntIntoEEPROM(6, timing);
  writeIntIntoEEPROM(9, DOF);
  writeIntIntoEEPROM(12, timingMove);
  writeIntIntoEEPROM(15, trigger);
  writeIntIntoEEPROM(18, mirror);
  writeIntIntoEEPROM(21, ShootMode);
  writeIntIntoEEPROM(24, AutoReturn);
  writeIntIntoEEPROM(27, motorSpeed);
  writeIntIntoEEPROM(30, added);
  writeIntIntoEEPROM(33, bootVal);
  writeIntIntoEEPROM(66, autoSave);
  delay(100);
  presetVal = 1;

  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print(F("MacroRail"));
  lcd.setCursor(1, 1);
  lcd.print(F("preset chosen"));
  delay(1750);

  actualState = 0;
  motorState = 0;
}

//------------------------------------------------------
void SetModeM() {
  presetVal = 0;
  stepsI = readIntFromEEPROM(36);
  timing = readIntFromEEPROM(39);
  DOF = readIntFromEEPROM(42);
  timingMove = readIntFromEEPROM(45);
  trigger = readIntFromEEPROM(48);
  mirror = readIntFromEEPROM(51);
  ShootMode = readIntFromEEPROM(54);
  AutoReturn = readIntFromEEPROM(57);
  motorSpeed = readIntFromEEPROM(60);
  added = readIntFromEEPROM(63);
  bootVal = readIntFromEEPROM(33);
  autoSave = readIntFromEEPROM(66);
  delay(100);
  writeIntIntoEEPROM(36, stepsI);
  writeIntIntoEEPROM(39, timing);
  writeIntIntoEEPROM(42, DOF);
  writeIntIntoEEPROM(45, timingMove);
  writeIntIntoEEPROM(48, trigger);
  writeIntIntoEEPROM(51, mirror);
  writeIntIntoEEPROM(54, ShootMode);
  writeIntIntoEEPROM(57, AutoReturn);
  writeIntIntoEEPROM(60, motorSpeed);
  writeIntIntoEEPROM(63, added);
  writeIntIntoEEPROM(33, bootVal);
  writeIntIntoEEPROM(66, autoSave);
  delay(100);
  presetVal = 0;

  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print(F("Microscope"));
  lcd.setCursor(1, 1);
  lcd.print(F("preset chosen"));
  delay(1750);

  actualState = 0;
  motorState = 0;
}

//------------------------------------------------------
void SetDOF() {
  lcd.clear();


  switch (presetVal) {
    case 0:
      switch (stepsI) {
        case 0:
          lcd.setCursor(0, 0);
          lcd.print(F("nm/step - normal"));
          delay(1750);
          DOF = lcd.inputVal<int>("Input nm/step", 10, 1800, DOF, 5);
          break;

        case 1:
          lcd.setCursor(0, 0);
          lcd.print(F("nm/step - HiRes"));
          delay(1750);
          DOF = lcd.inputVal<int>("Input nm/step", 5, 1000, DOF, 1);
          break;
      }
      break;

    case 1:
      switch (stepsI) {
        case 0:
          lcd.setCursor(0, 0);
          lcd.print(F("um/step - normal"));
          delay(1750);
          DOF = lcd.inputVal<int>("Input um/step", 20, 1500, DOF, 5);
          break;

        case 1:
          lcd.setCursor(0, 0);
          lcd.print(F("um/step - HiRes"));
          delay(1750);
          DOF = lcd.inputVal<int>("Input um/step", 10, 652, DOF, 2);
          break;
      }
      break;
  }




  /*
    switch (stepsI) {
      case 0:
        lcd.setCursor(0, 0);
        lcd.print(F("um/step"));
        DOF = lcd.inputVal<int>("Input um/step", 20, 1500, DOF, 5);
        break;

      case 1:
        lcd.setCursor(0, 0);
        lcd.print(F("um/step"));
        DOF = lcd.inputVal<int>("Input um/step", 10, 652, DOF, 2);
        break;
    }
  */

  switch (autoSave) {
    case 0:
      switch (presetVal) {
        case 0:
          delay(100);
          break;

        case 1:
          delay(100);
          break;
      }
      break;

    case 1:
      switch (presetVal) {
        case 0:
          writeIntIntoEEPROM(42, DOF);
          delay(200);
          break;

        case 1:
          writeIntIntoEEPROM(9, DOF);
          delay(200);
          break;
      }
      break;
  }

}

//------------------------------------------------------
void SetBoot() {
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(F("enable/disable"));
  lcd.setCursor(0, 1);
  lcd.print(F("init. BOOTscreen"));
  delay(1750);
  bootVal = lcd.inputVal<int>("0 = OFF / 1 = ON", 0, 1, bootVal);
  writeIntIntoEEPROM(33, bootVal);
  delay(500);
}

//------------------------------------------------------
void SetPresetM() {
  presetVal = 0;

  stepsI = readIntFromEEPROM(36);
  timing = readIntFromEEPROM(39);
  DOF = readIntFromEEPROM(42);
  timingMove = readIntFromEEPROM(45);
  trigger = readIntFromEEPROM(48);
  mirror = readIntFromEEPROM(51);
  ShootMode = readIntFromEEPROM(54);
  AutoReturn = readIntFromEEPROM(57);
  motorSpeed = readIntFromEEPROM(60);
  added = readIntFromEEPROM(63);
  bootVal = readIntFromEEPROM(33);
  autoSave = readIntFromEEPROM(66);
  delay(100);
  writeIntIntoEEPROM(36, stepsI);
  writeIntIntoEEPROM(39, timing);
  writeIntIntoEEPROM(42, DOF);
  writeIntIntoEEPROM(45, timingMove);
  writeIntIntoEEPROM(48, trigger);
  writeIntIntoEEPROM(51, mirror);
  writeIntIntoEEPROM(54, ShootMode);
  writeIntIntoEEPROM(57, AutoReturn);
  writeIntIntoEEPROM(60, motorSpeed);
  writeIntIntoEEPROM(63, added);
  writeIntIntoEEPROM(33, bootVal);
  writeIntIntoEEPROM(70, presetVal);
  writeIntIntoEEPROM(66, autoSave);
  delay(100);
  presetVal = 0;

  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print(F("Microscope"));
  lcd.setCursor(1, 1);
  lcd.print(F("preset chosen"));
  delay(1750);

  actualState = 0;
  motorState = 0;
}

//------------------------------------------------------


void SetPresetR() {
  presetVal = 1;

  stepsI = readIntFromEEPROM(3);
  timing = readIntFromEEPROM(6);
  DOF = readIntFromEEPROM(9);
  timingMove = readIntFromEEPROM(12);
  trigger = readIntFromEEPROM(15);
  mirror = readIntFromEEPROM(18);
  ShootMode = readIntFromEEPROM(21);
  AutoReturn = readIntFromEEPROM(24);
  motorSpeed = readIntFromEEPROM(27);
  added = readIntFromEEPROM(30);
  bootVal = readIntFromEEPROM(33);
  autoSave = readIntFromEEPROM(66);
  delay(100);
  writeIntIntoEEPROM(3, stepsI);
  writeIntIntoEEPROM(6, timing);
  writeIntIntoEEPROM(9, DOF);
  writeIntIntoEEPROM(12, timingMove);
  writeIntIntoEEPROM(15, trigger);
  writeIntIntoEEPROM(18, mirror);
  writeIntIntoEEPROM(21, ShootMode);
  writeIntIntoEEPROM(24, AutoReturn);
  writeIntIntoEEPROM(27, motorSpeed);
  writeIntIntoEEPROM(30, added);
  writeIntIntoEEPROM(33, bootVal);
  writeIntIntoEEPROM(70, presetVal);
  writeIntIntoEEPROM(66, autoSave);
  delay(100);
  presetVal = 1;

  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print(F("MacroRail"));
  lcd.setCursor(1, 1);
  lcd.print(F("preset chosen"));
  delay(1750);

  actualState = 0;
  motorState = 0;
}

//------------------------------------------------------
void SetShootMode() {
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(F("enable/disable"));
  lcd.setCursor(0, 1);
  lcd.print(F("mirrorLOCK func."));
  delay(1750);
  ShootMode = lcd.inputVal<int>("0 = OFF / 1 = ON", 0, 1, ShootMode);

  switch (autoSave) {
    case 0:
      switch (presetVal) {
        case 0:
          delay(100);
          break;

        case 1:
          delay(100);
          break;
      }
      break;

    case 1:
      switch (presetVal) {
        case 0:
          writeIntIntoEEPROM(54, ShootMode);
          delay(200);
          break;

        case 1:
          writeIntIntoEEPROM(21, ShootMode);
          delay(200);
          break;
      }
      break;
  }
}

//------------------------------------------------------
void SetAdded() {
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print(F("set length"));
  lcd.setCursor(1, 1);
  lcd.print(F("added to move"));
  delay(1750);
  added = lcd.inputVal<int>("Input um", 10, 100, added, 10);

  switch (autoSave) {
    case 0:
      switch (presetVal) {
        case 0:
          delay(100);
          break;

        case 1:
          delay(100);
          break;
      }
      break;

    case 1:
      switch (presetVal) {
        case 0:
          writeIntIntoEEPROM(63, added);
          delay(200);
          break;

        case 1:
          writeIntIntoEEPROM(30, added);
          delay(200);
          break;
      }
      break;
  }
}

//------------------------------------------------------
void SetAutoReturn() {
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(F("enable/disable"));
  lcd.setCursor(3, 1);
  lcd.print(F("after stack"));
  delay(1750);
  AutoReturn = lcd.inputVal<int>("0 = OFF / 1 = ON", 0, 1, AutoReturn);

  switch (autoSave) {
    case 0:
      switch (presetVal) {
        case 0:
          delay(100);
          break;

        case 1:
          delay(100);
          break;
      }
      break;

    case 1:
      switch (presetVal) {
        case 0:
          writeIntIntoEEPROM(57, AutoReturn);
          delay(200);
          break;

        case 1:
          writeIntIntoEEPROM(24, AutoReturn);
          delay(200);
          break;
      }
      break;
  }
}

//------------------------------------------------------
void SetDelay() {
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(F("shutter speed"));
  lcd.setCursor(0, 1);
  lcd.print(F("per single photo"));
  delay(1750);
  timing = lcd.inputVal<int>("Input delay (s)", 1, 20, timing);

  switch (autoSave) {
    case 0:
      switch (presetVal) {
        case 0:
          delay(100);
          break;

        case 1:
          delay(100);
          break;
      }
      break;

    case 1:
      switch (presetVal) {
        case 0:
          writeIntIntoEEPROM(39, timing);
          delay(200);
          break;

        case 1:
          writeIntIntoEEPROM(6, timing);
          delay(200);
          break;
      }
      break;
  }
}

//------------------------------------------------------
void SetDelayMove() {
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(F("delay between"));
  lcd.setCursor(1, 1);
  lcd.print(F("move and photo"));
  delay(1750);
  timingMove = lcd.inputVal<int>("Input delay (s)", 1, 20, timingMove);

  switch (autoSave) {
    case 0:
      switch (presetVal) {
        case 0:
          delay(100);
          break;

        case 1:
          delay(100);
          break;
      }
      break;

    case 1:
      switch (presetVal) {
        case 0:
          writeIntIntoEEPROM(45, timingMove);
          delay(200);
          break;

        case 1:
          writeIntIntoEEPROM(12, timingMove);
          delay(200);
          break;
      }
      break;
  }
}

//------------------------------------------------------
void SetDelayTrigger() {
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(F("adjust remote"));
  lcd.setCursor(1, 1);
  lcd.print(F("trigger length"));
  delay(1750);
  trigger = lcd.inputVal<int>("Input delay (us)", 250, 2000, trigger, 250);

  switch (autoSave) {
    case 0:
      switch (presetVal) {
        case 0:
          delay(100);
          break;

        case 1:
          delay(100);
          break;
      }
      break;

    case 1:
      switch (presetVal) {
        case 0:
          writeIntIntoEEPROM(48, trigger);
          delay(200);
          break;

        case 1:
          writeIntIntoEEPROM(15, trigger);
          delay(200);
          break;
      }
      break;
  }
}

//------------------------------------------------------
void SetMirror() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("time to prevent"));
  lcd.setCursor(0, 1);
  lcd.print(F("mirror vibration"));
  delay(1750);
  mirror = lcd.inputVal<int>("Input delay (us)", 250, 5000, mirror, 250);

  switch (autoSave) {
    case 0:
      switch (presetVal) {
        case 0:
          delay(100);
          break;

        case 1:
          delay(100);
          break;
      }
      break;

    case 1:
      switch (presetVal) {
        case 0:
          writeIntIntoEEPROM(51, mirror);
          delay(200);
          break;

        case 1:
          writeIntIntoEEPROM(18, mirror);
          delay(200);
          break;
      }
      break;
  }
}


//------------------------------------------------------
void SetSave() {
  //set preset Rail
  if (presetVal == 1) {
    writeIntIntoEEPROM(3, stepsI);
    writeIntIntoEEPROM(6, timing);
    writeIntIntoEEPROM(9, DOF);
    writeIntIntoEEPROM(12, timingMove);
    writeIntIntoEEPROM(15, trigger);
    writeIntIntoEEPROM(18, mirror);
    writeIntIntoEEPROM(21, ShootMode);
    writeIntIntoEEPROM(24, AutoReturn);
    writeIntIntoEEPROM(27, motorSpeed);
    writeIntIntoEEPROM(30, added);
    writeIntIntoEEPROM(33, bootVal);
    writeIntIntoEEPROM(70, presetVal);
    writeIntIntoEEPROM(66, autoSave);
    delay(200);
  }

  //set preset Microscope
  if (presetVal == 0) {
    writeIntIntoEEPROM(36, stepsI);
    writeIntIntoEEPROM(39, timing);
    writeIntIntoEEPROM(42, DOF);
    writeIntIntoEEPROM(45, timingMove);
    writeIntIntoEEPROM(48, trigger);
    writeIntIntoEEPROM(51, mirror);
    writeIntIntoEEPROM(54, ShootMode);
    writeIntIntoEEPROM(57, AutoReturn);
    writeIntIntoEEPROM(60, motorSpeed);
    writeIntIntoEEPROM(63, added);
    writeIntIntoEEPROM(33, bootVal);
    writeIntIntoEEPROM(70, presetVal);
    writeIntIntoEEPROM(66, autoSave);
    delay(200);
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("saved to EEPROM"));
  delay(1750);
}


//------------------------------------------------------
/*
  void SetSaveFS() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("FocusStack saved"));
  delay(750);

  writeIntIntoEEPROM(3, stepsI);
  writeIntIntoEEPROM(6, timing);
  writeIntIntoEEPROM(9, DOF);
  writeIntIntoEEPROM(12, timingMove);
  writeIntIntoEEPROM(15, trigger);
  writeIntIntoEEPROM(18, mirror);
  writeIntIntoEEPROM(21, ShootMode);
  writeIntIntoEEPROM(24, AutoReturn);
  writeIntIntoEEPROM(27, motorSpeed);
  writeIntIntoEEPROM(30, added);
  writeIntIntoEEPROM(33, bootVal);
  writeIntIntoEEPROM(70, presetVal);
  delay(wait);
  }

  //------------------------------------------------------
  void SetSaveM() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Microscope saved"));
  delay(750);

  writeIntIntoEEPROM(36, stepsI);
  writeIntIntoEEPROM(39, timing);
  writeIntIntoEEPROM(42, DOF);
  writeIntIntoEEPROM(45, timingMove);
  writeIntIntoEEPROM(48, trigger);
  writeIntIntoEEPROM(51, mirror);
  writeIntIntoEEPROM(54, ShootMode);
  writeIntIntoEEPROM(57, AutoReturn);
  writeIntIntoEEPROM(60, motorSpeed);
  writeIntIntoEEPROM(63, added);
  writeIntIntoEEPROM(66, bootVal);
  writeIntIntoEEPROM(70, presetVal);
  delay(wait);
  }
*/
//------------------------------------------------------
void SetDefaultsFS() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Rail loaded"));
  delay(750);

  stepsI = 0;
  timing = 2;
  DOF = 250;
  timingMove = 2;
  trigger = 500;
  mirror = 750;
  ShootMode = 0;
  AutoReturn = 1;
  motorSpeed = 500;
  added = 30;
  bootVal = 1;
  presetVal = 1;
  autoSave = 0;

  delay(100);
  writeIntIntoEEPROM(3, stepsI);
  writeIntIntoEEPROM(6, timing);
  writeIntIntoEEPROM(9, DOF);
  writeIntIntoEEPROM(12, timingMove);
  writeIntIntoEEPROM(15, trigger);
  writeIntIntoEEPROM(18, mirror);
  writeIntIntoEEPROM(21, ShootMode);
  writeIntIntoEEPROM(24, AutoReturn);
  writeIntIntoEEPROM(27, motorSpeed);
  writeIntIntoEEPROM(30, added);
  writeIntIntoEEPROM(33, bootVal);
  writeIntIntoEEPROM(70, presetVal);
  writeIntIntoEEPROM(66, autoSave);
  delay(100);
}

//------------------------------------------------------
void SetDefaultsM() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Micro. loaded"));
  delay(750);

  stepsI = 1;
  timing = 3;
  DOF = 20; //250
  timingMove = 2;
  trigger = 500;
  mirror = 750;
  ShootMode = 0;
  AutoReturn = 1;
  motorSpeed = 500;
  added = 50;
  bootVal = 1;
  presetVal = 0;
  autoSave = 0;

  delay(100);
  writeIntIntoEEPROM(36, stepsI);
  writeIntIntoEEPROM(39, timing);
  writeIntIntoEEPROM(42, DOF);
  writeIntIntoEEPROM(45, timingMove);
  writeIntIntoEEPROM(48, trigger);
  writeIntIntoEEPROM(51, mirror);
  writeIntIntoEEPROM(54, ShootMode);
  writeIntIntoEEPROM(57, AutoReturn);
  writeIntIntoEEPROM(60, motorSpeed);
  writeIntIntoEEPROM(63, added);
  writeIntIntoEEPROM(33, bootVal);
  writeIntIntoEEPROM(70, presetVal);
  writeIntIntoEEPROM(66, autoSave);
  delay(100);
}

//------------------------------------------------------
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//------------------------------------------------------
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
void setup() {
  lcd.begin();
  lcd.attachEncoder(pinCLK, pinDT, pinSW);
  delay(500);
  presetVal = readIntFromEEPROM(70);

  delay(500);

  //load preset FS
  if (presetVal == 1) {
    stepsI = readIntFromEEPROM(3);
    timing = readIntFromEEPROM(6);
    DOF = readIntFromEEPROM(9);
    timingMove = readIntFromEEPROM(12);
    trigger = readIntFromEEPROM(15);
    mirror = readIntFromEEPROM(18);
    ShootMode = readIntFromEEPROM(21);
    AutoReturn = readIntFromEEPROM(24);
    motorSpeed = readIntFromEEPROM(27);
    added = readIntFromEEPROM(30);
    bootVal = readIntFromEEPROM(33);
    autoSave = readIntFromEEPROM(66);
    delay(200);
    presetVal = 1;
  }

  //load preset Microscope
  if (presetVal == 0) {
    stepsI = readIntFromEEPROM(36);
    timing = readIntFromEEPROM(39);
    DOF = readIntFromEEPROM(42);
    timingMove = readIntFromEEPROM(45);
    trigger = readIntFromEEPROM(48);
    mirror = readIntFromEEPROM(51);
    ShootMode = readIntFromEEPROM(54);
    AutoReturn = readIntFromEEPROM(57);
    motorSpeed = readIntFromEEPROM(60);
    added = readIntFromEEPROM(63);
    bootVal = readIntFromEEPROM(33);
    autoSave = readIntFromEEPROM(66);
    delay(200);
    presetVal = 0;
  }

  delay(200);

  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(enable, OUTPUT);
  pinMode(msg1, OUTPUT);
  pinMode(msg2, OUTPUT);
  pinMode(msg3, OUTPUT);
  pinMode(releTrig, OUTPUT);

  //------------------------------------------------
  //BOOT SCREEN
  if (bootVal == 1) {
    lcd.setCursor(3, 0);
    lcd.print(F("fccFS2 PRO"));
    for (int letter = 0; letter <= strlen(messagePadded) - 16; letter++) {
      showLetters(0, letter);
    }
  }
  if (bootVal == 0) {
    delay(300);
  }

  lcd.clear();
  LCDRepaint();
  focusCounter = 1;  //1 due restriction of division with 0

  releON();
  delay(500);
  releOFF();
  delay(100);

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

  switch (stepsI) {
    case 0:
      setStep1 = 1;
      setStep2 = 1;
      setStep3 = 0;  //1/16
      newStepsI = 0;
      break;

    case 1:
      setStep1 = 1;
      setStep2 = 1;  //1/32
      setStep3 = 1;
      newStepsI = 1;
      break;
  }

  //------------------------------------------------------
  // IF 1 PRESSED (GO BACK)
  if (customKey == '1') {

    switch (stepsI) {
      case 0:
        motorDelay = motorSpeed * 2;
        break;

      case 1:
        motorDelay = motorSpeed;
        break;
    }


    for (;;) {
      char customKey = keypad.getKey();
      goFront();
      if (actualState == 0) {  //to not have negative value
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

  //------------------------------------------------------
  // IF 2 PRESSED (SAVE BACK)
  if (customKey == '2') {
    motorState = 0;
    delay(10);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("START POSITION"));
    lcd.setCursor(0, 1);
    lcd.print(F("SAVED"));
    delay(wait);  //length of showing -- can be set at top of code
    actualState = 0;
    LCDRepaint();
  }

  //------------------------------------------------------
  // IF 3 PRESSED (GO FRONT)
  if (customKey == '3') {
    switch (stepsI) {
      case 0:
        motorDelay = motorSpeed * 2;
        break;

      case 1:
        motorDelay = motorSpeed;
        break;
    }

    for (;;) {
      char customKey = keypad.getKey();
      goBack();
      if (customKey == '3') {
        break;
        LCDRepaint();
        delay(30);
      }
    }
  }

  //------------------------------------------------------
  // IF 4 PRESSED (SAVE FRONT)
  if (customKey == '4') {
    motorDelay = motorSpeed;
    motorState = (actualState + added / 2);

    delay(10);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("END POSITION"));
    lcd.setCursor(0, 1);
    lcd.print(F("SAVED"));
    delay(wait);  //length of showing -- can be set at top of code
    LCDRepaint();
    delay(1000);

    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print(F("Moving to the"));
    lcd.setCursor(1, 1);
    lcd.print(F("START position"));
    delay(400);

    for (int i = 0; i < motorState; i++) {
      goFront2();
    }
    motorState = (motorState + added);
    delay(300);
    LCDRepaint();
  }

  //------------------------------------------------------
  // knob button
  if (lcd.getEncoderState() == eButton) {
    lcd.showMenu(menu, menuLen, 1);
    LCDRepaint();
  }
}
