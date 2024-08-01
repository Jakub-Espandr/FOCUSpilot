#include <Wire.h>
#include <LiquidCrystal_I2C_Menu.h>
#include <EEPROM.h>
#include <Keypad.h>
#include <EEPROMTyped.h>

// Initialize LCD with I2C address 0x27, 16 columns and 2 rows
LiquidCrystal_I2C_Menu lcd(0x27, 16, 2);

//------------------------------------------------------
// User definitions
#define wait 500

// Pin definitions
#define pinCLK 2
#define pinDT 4
#define pinSW 3
#define stepPin 13
#define dirPin 12
#define enable 17
#define msg1 16
#define msg2 15
#define msg3 14
#define releTrig 10

//------------------------------------------------------
// EEPROM addresses for microscope (each variable has 2 bytes allocated)
#define ADDR_MICRO_STEPSI 0       // 2 bytes
#define ADDR_MICRO_TIMING 2       // 2 bytes
#define ADDR_MICRO_DOF 4          // 2 bytes
#define ADDR_MICRO_TIMINGMOVE 6   // 2 bytes
#define ADDR_MICRO_TRIGGER 8      // 2 bytes
#define ADDR_MICRO_MIRROR 10      // 2 bytes
#define ADDR_MICRO_SHOOTMODE 12   // 2 bytes
#define ADDR_MICRO_AUTORETURN 14  // 2 bytes
#define ADDR_MICRO_MOTORSPEED 16  // 2 bytes
#define ADDR_MICRO_ADDED 18       // 2 bytes
#define ADDR_MICRO_BOOTVAL 20     // 2 bytes
#define ADDR_MICRO_PRESETVAL 22   // 2 bytes
#define ADDR_MICRO_AUTOSAVE 24    // 2 bytes
#define ADDR_objective1C1 30
#define ADDR_objective2C1 32
#define ADDR_objective3C1 34
#define ADDR_objective4C1 36
#define ADDR_objective5C1 38

#define ADDR_objective1C2 40
#define ADDR_objective2C2 42
#define ADDR_objective3C2 44
#define ADDR_objective4C2 46
#define ADDR_objective5C2 48

#define ADDR_objective1C3 50
#define ADDR_objective2C3 52
#define ADDR_objective3C3 54
#define ADDR_objective4C3 56
#define ADDR_objective5C3 58

#define ADDR_MICRO_NUMPHOTOS 60  // 2 bytes for number of photos

//------------------------------------------------------
// EEPROM functions
void writeIntIntoEEPROM(int address, int number) {
  EEPROM.write(address, number >> 8);
  EEPROM.write(address + 1, number & 0xFF);
}

int readIntFromEEPROM(int address) {
  return (EEPROM.read(address) << 8) + EEPROM.read(address + 1);
}

//------------------------------------------------------
// Display startup screen
const char messagePadded[] PROGMEM = "        v1.2.0.                ";  // Pointer to message stored in flash memory

void showLetters(int printStart, int startLetter) {
  lcd.setCursor(printStart, 1);
  for (int letter = startLetter; letter <= startLetter + 15; letter++) {
    lcd.print((char)pgm_read_byte(&(messagePadded[letter])));
  }
  lcd.print(" ");
  delay(310);
}

//------------------------------------------------------
// Keypad
const byte ROWS = 1;
const byte COLS = 4;
const char keys[ROWS][COLS] = {
  { '1', '2', '3', '4' }  //4 3 2 1
};
byte rowPins[ROWS] = { 5 };           // Keypad row pins
byte colPins[COLS] = { 7, 6, 9, 8 };  // Keypad column pins
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

//------------------------------------------------------
// Group related variables into structure
struct Settings {
  int16_t DOF;
  uint16_t localDOF;
  int8_t localAdded;
  int8_t percentage;
  int8_t stepsI;
  int8_t ShootMode;
  int8_t AutoReturn;
  int8_t newStepsI;
  int16_t timing;
  int16_t timingMove;
  int16_t motorSpeed;
  int16_t motorDelay;
  int8_t added;
  int16_t delayTiming;
  int16_t delayTimingMove;
  int16_t delayTimingTrigger;
  int16_t trigger;
  int16_t mirror;
  int8_t bootVal;
  int8_t presetVal;
  int8_t autoSave;
  int16_t valueO1C1;
  int16_t valueO2C1;
  int16_t valueO3C1;
  int16_t valueO4C1;
  int16_t valueO5C1;
  int16_t valueO1C2;
  int16_t valueO2C2;
  int16_t valueO3C2;
  int16_t valueO4C2;
  int16_t valueO5C2;
  int16_t valueO1C3;
  int16_t valueO2C3;
  int16_t valueO3C3;
  int16_t valueO4C3;
  int16_t valueO5C3;
  int16_t numPhotos;  // New variable for storing number of requested photos
} settings;

// Group status variables into structure
struct Status {
  uint16_t actualState;
  uint16_t motorState;
  uint16_t focusMotorState;
  uint16_t focusCounter;
  uint16_t loopCounter;
  int8_t emergency;
} status;

// Group all boolean variables into a single byte
uint8_t stepSettings;  // bit 0: setStep1, bit 1: setStep2, bit 2: setStep3
#define SET_STEP1 (stepSettings & 0x01)
#define SET_STEP2 (stepSettings & 0x02)
#define SET_STEP3 (stepSettings & 0x04)

int8_t firstRun = 0;

//------------------------------------------------------
// Menu enum
enum {
  menuBack,
  menuRoot,
  menuShot,
  menuStep,
  menuSettings,
  menuPreferences,
  menuDOF,
  menuTest,
  menuTestStep,
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
  menuSaveMicroscope,
  menuRestore,
  menuRestoreMicroscope,
  menuDefaults,
  menuBoot,
  menuPreset,
  menuPresetM,
  menuInfo,
  menuInfoDisp,
  menuRun,
  menuRunF,
  menuRunB,
  menuRunFsettings,
  menuRunBsettings,
  menuDelete,
  menuAutoSave,
  menuSetObjectives,
  chooseObjective,
  chooseCarousel1,
  setCarousel1,
  getObjective1C1_menu,
  getObjective2C1_menu,
  getObjective3C1_menu,
  getObjective4C1_menu,
  getObjective5C1_menu,
  setObjective1C1_menu,
  setObjective2C1_menu,
  setObjective3C1_menu,
  setObjective4C1_menu,
  setObjective5C1_menu,
  chooseCarousel2,
  setCarousel2,
  setObjective1C2_menu,
  setObjective2C2_menu,
  setObjective3C2_menu,
  setObjective4C2_menu,
  setObjective5C2_menu,
  getObjective1C2_menu,
  getObjective2C2_menu,
  getObjective3C2_menu,
  getObjective4C2_menu,
  getObjective5C2_menu,
  chooseCarousel3,
  setCarousel3,
  setObjective1C3_menu,
  setObjective2C3_menu,
  setObjective3C3_menu,
  setObjective4C3_menu,
  setObjective5C3_menu,
  getObjective1C3_menu,
  getObjective2C3_menu,
  getObjective3C3_menu,
  getObjective4C3_menu,
  getObjective5C3_menu,
  menuRunByPhotoCount,  // New menu option for running based on photo count
  menuNumPhotos         // New menu option for setting number of photos
};

//------------------------------------------------------
// Menu function prototypes
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
void SetDefaultsM();
void SetSpeed();
void SetSave();
void delTrace();
void capture();
void SetBoot();
void SetAutoSave();
void releON();
void releOFF();
void motorStop();
void LCDRepaint();
void getObjective1C1();
void setObjective1C1();
void getObjective2C1();
void setObjective2C1();
void getObjective3C1();
void setObjective3C1();
void getObjective4C1();
void setObjective4C1();
void getObjective5C1();
void setObjective5C1();

void setObjective1C2();
void getObjective1C2();
void setObjective2C2();
void getObjective2C2();
void setObjective3C2();
void getObjective3C2();
void setObjective4C2();
void getObjective4C2();
void setObjective5C2();
void getObjective5C2();

void setObjective1C3();
void getObjective1C3();
void setObjective2C3();
void getObjective2C3();
void setObjective3C3();
void getObjective3C3();
void setObjective4C3();
void getObjective4C3();
void setObjective5C3();
void getObjective5C3();
void SetNumPhotos();       // New function for setting number of photos
void goRunByPhotoCount();  // New function for running based on photo count

//------------------------------------------------------
// Functions
void releON() {
  digitalWrite(releTrig, LOW);
}

void releOFF() {
  digitalWrite(releTrig, HIGH);
}

// Set objective functions for C1
void setObjective1C1() {
  settings.valueO1C1 = lcd.inputVal<int>("Input in ms", 5, 1250, settings.valueO1C1, 5);
  writeIntIntoEEPROM(ADDR_objective1C1, settings.valueO1C1);
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(F("Objective 1 set"));
  delay(1000);
  LCDRepaint();
}

void getObjective1C1() {
  settings.DOF = readIntFromEEPROM(ADDR_objective1C1);
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(F("Objective 1 set"));
  delay(1000);
  LCDRepaint();
}

void setObjective2C1() {
  settings.valueO2C1 = lcd.inputVal<int>("Input in ms", 5, 1250, settings.valueO2C1, 5);
  writeIntIntoEEPROM(ADDR_objective2C1, settings.valueO2C1);
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(F("Objective 2 set"));
  delay(1000);
  LCDRepaint();
}

void getObjective2C1() {
  settings.DOF = readIntFromEEPROM(ADDR_objective2C1);
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(F("Objective 2 set"));
  delay(1000);
  LCDRepaint();
}

void setObjective3C1() {
  settings.valueO3C1 = lcd.inputVal<int>("Input in ms", 5, 1250, settings.valueO3C1, 5);
  writeIntIntoEEPROM(ADDR_objective3C1, settings.valueO3C1);
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(F("Objective 3 set"));
  delay(1000);
  LCDRepaint();
}

void getObjective3C1() {
  settings.DOF = readIntFromEEPROM(ADDR_objective3C1);
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(F("Objective 3 set"));
  delay(1000);
  LCDRepaint();
}

void setObjective4C1() {
  settings.valueO4C1 = lcd.inputVal<int>("Input in ms", 5, 1250, settings.valueO4C1, 5);
  writeIntIntoEEPROM(ADDR_objective4C1, settings.valueO4C1);
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(F("Objective 4 set"));
  delay(1000);
  LCDRepaint();
}

void getObjective4C1() {
  settings.DOF = readIntFromEEPROM(ADDR_objective4C1);
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(F("Objective 4 set"));
  delay(1000);
  LCDRepaint();
}

void setObjective5C1() {
  settings.valueO5C1 = lcd.inputVal<int>("Input in ms", 5, 1250, settings.valueO5C1, 5);
  writeIntIntoEEPROM(ADDR_objective5C1, settings.valueO5C1);
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(F("Objective 5 set"));
  delay(1000);
  LCDRepaint();
}

void getObjective5C1() {
  settings.DOF = readIntFromEEPROM(ADDR_objective5C1);
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(F("Objective 5 set"));
  delay(1000);
  LCDRepaint();
}

// Set objective functions for C2
void setObjective1C2() {
  settings.valueO1C2 = lcd.inputVal<int>("Input in ms", 5, 1250, settings.valueO1C2, 5);
  writeIntIntoEEPROM(ADDR_objective1C2, settings.valueO1C2);
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(F("Objective 1 set"));
  delay(1000);
  LCDRepaint();
}

void getObjective1C2() {
  settings.DOF = readIntFromEEPROM(ADDR_objective1C2);
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(F("Objective 1 set"));
  delay(1000);
  LCDRepaint();
}

void setObjective2C2() {
  settings.valueO2C2 = lcd.inputVal<int>("Input in ms", 5, 1250, settings.valueO2C2, 5);
  writeIntIntoEEPROM(ADDR_objective2C2, settings.valueO2C2);
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(F("Objective 2 set"));
  delay(1000);
  LCDRepaint();
}

void getObjective2C2() {
  settings.DOF = readIntFromEEPROM(ADDR_objective2C2);
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(F("Objective 2 set"));
  delay(1000);
  LCDRepaint();
}

void setObjective3C2() {
  settings.valueO3C2 = lcd.inputVal<int>("Input in ms", 5, 1250, settings.valueO3C2, 5);
  writeIntIntoEEPROM(ADDR_objective3C2, settings.valueO3C2);
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(F("Objective 3 set"));
  delay(1000);
  LCDRepaint();
}

void getObjective3C2() {
  settings.DOF = readIntFromEEPROM(ADDR_objective3C2);
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(F("Objective 3 set"));
  delay(1000);
  LCDRepaint();
}

void setObjective4C2() {
  settings.valueO4C2 = lcd.inputVal<int>("Input in ms", 5, 1250, settings.valueO4C2, 5);
  writeIntIntoEEPROM(ADDR_objective4C2, settings.valueO4C2);
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(F("Objective 4 set"));
  delay(1000);
  LCDRepaint();
}

void getObjective4C2() {
  settings.DOF = readIntFromEEPROM(ADDR_objective4C2);
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(F("Objective 4 set"));
  delay(1000);
  LCDRepaint();
}

void setObjective5C2() {
  settings.valueO5C2 = lcd.inputVal<int>("Input in ms", 5, 1250, settings.valueO5C2, 5);
  writeIntIntoEEPROM(ADDR_objective5C2, settings.valueO5C2);
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(F("Objective 5 set"));
  delay(1000);
  LCDRepaint();
}

void getObjective5C2() {
  settings.DOF = readIntFromEEPROM(ADDR_objective5C2);
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(F("Objective 5 set"));
  delay(1000);
  LCDRepaint();
}

// Set objective functions for C3
void setObjective1C3() {
  settings.valueO1C3 = lcd.inputVal<int>("Input in ms", 5, 1250, settings.valueO1C3, 5);
  writeIntIntoEEPROM(ADDR_objective1C3, settings.valueO1C3);
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(F("Objective 1 set"));
  delay(1000);
  LCDRepaint();
}

void getObjective1C3() {
  settings.DOF = readIntFromEEPROM(ADDR_objective1C3);
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(F("Objective 1 set"));
  delay(1000);
  LCDRepaint();
}

void setObjective2C3() {
  settings.valueO2C3 = lcd.inputVal<int>("Input in ms", 5, 1250, settings.valueO2C3, 5);
  writeIntIntoEEPROM(ADDR_objective2C3, settings.valueO2C3);
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(F("Objective 2 set"));
  delay(1000);
  LCDRepaint();
}

void getObjective2C3() {
  settings.DOF = readIntFromEEPROM(ADDR_objective2C3);
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(F("Objective 2 set"));
  delay(1000);
  LCDRepaint();
}

void setObjective3C3() {
  settings.valueO3C3 = lcd.inputVal<int>("Input in ms", 5, 1250, settings.valueO3C3, 5);
  writeIntIntoEEPROM(ADDR_objective3C3, settings.valueO3C3);
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(F("Objective 3 set"));
  delay(1000);
  LCDRepaint();
}

void getObjective3C3() {
  settings.DOF = readIntFromEEPROM(ADDR_objective3C3);
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(F("Objective 3 set"));
  delay(1000);
  LCDRepaint();
}

void setObjective4C3() {
  settings.valueO4C3 = lcd.inputVal<int>("Input in ms", 5, 1250, settings.valueO4C3, 5);
  writeIntIntoEEPROM(ADDR_objective4C3, settings.valueO4C3);
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(F("Objective 4 set"));
  delay(1000);
  LCDRepaint();
}

void getObjective4C3() {
  settings.DOF = readIntFromEEPROM(ADDR_objective4C3);
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(F("Objective 4 set"));
  delay(1000);
  LCDRepaint();
}

void setObjective5C3() {
  settings.valueO5C3 = lcd.inputVal<int>("Input in ms", 5, 1250, settings.valueO5C3, 5);
  writeIntIntoEEPROM(ADDR_objective5C3, settings.valueO5C3);
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(F("Objective 5 set"));
  delay(1000);
  LCDRepaint();
}

void getObjective5C3() {
  settings.DOF = readIntFromEEPROM(ADDR_objective5C3);
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(F("Objective 5 set"));
  delay(1000);
  LCDRepaint();
}

// Function for setting number of photos
void SetNumPhotos() {
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print(F("Set Number"));
  lcd.setCursor(0, 1);
  lcd.print(F("of Photos"));
  delay(1750);

  settings.numPhotos = lcd.inputVal<int>("Input number", 1, 100, settings.numPhotos);

  if (settings.autoSave == 1) {
    writeIntIntoEEPROM(ADDR_MICRO_NUMPHOTOS, settings.numPhotos);  // Use new EEPROM address for saving numPhotos
    delay(200);
  }
}

// Function for running based on photo count
void goRunByPhotoCount() {
  lcd.clear();

  // Calculate motor delay based on DOF
  if (settings.DOF >= 50) {
    settings.motorDelay = 800;
  } else {
    settings.motorDelay = 300;
  }

  status.loopCounter = 0;
  status.focusCounter = 0;
  status.emergency = 0;

  // Calculate step size based on requested number of photos
  if (settings.numPhotos > 0) {
    settings.localDOF = status.motorState / settings.numPhotos;  // Steps per photo
    status.focusMotorState = settings.numPhotos;                 // Set number of photos as number of steps to perform
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("Set num photos!"));
    delay(2000);
    LCDRepaint();
    return;
  }

  settings.delayTiming = settings.timing * 1000;
  settings.delayTimingMove = settings.timingMove * 1000;
  delay(2400);  // Wait before starting movement

  for (int i = 0; i < status.focusMotorState; i++) {
    status.loopCounter++;

    for (int j = 0; j < settings.localDOF; j++) {
      char customKey = keypad.getKey();

      if (digitalRead(3) == LOW) {
        status.emergency = 1;
        break;
        LCDRepaint();
        delay(30);
      }
      goBack();
      status.focusCounter++;
    }

    lcd.clear();
    int remainingTime = (((settings.trigger * 2) + settings.mirror + 5000 + settings.delayTiming + settings.delayTimingMove) / 2000);
    int laps = (status.focusMotorState - status.loopCounter);

    lcd.setCursor(0, 0);
    lcd.print(status.loopCounter);
    lcd.print(F("/"));
    lcd.print(status.focusMotorState);

    lcd.setCursor(10, 0);
    lcd.print(laps * remainingTime);
    lcd.print(F("s"));

    lcd.setCursor(0, 1);
    lcd.print(F("increment "));

    lcd.setCursor(10, 1);
    lcd.print(settings.DOF);
    lcd.print(F("nm"));

    delay(settings.delayTimingMove);
    capture();
    delay(settings.delayTiming);

    if (digitalRead(3) == LOW) {
      status.emergency = 1;
      break;
      LCDRepaint();
      delay(30);
      motorStop();
    }
  }

  if ((settings.AutoReturn == 1) && (status.emergency == 0)) {
    for (int i = 0; i < status.motorState; i++) {
      goFront();
    }
    motorStop();
  }

  status.emergency = 0;
  status.focusMotorState = 0;
  status.focusCounter = 0;
  motorStop();
}

// Function for triggering capture
void capture() {
  if (settings.ShootMode == 0) {
    releON();
    delay(settings.trigger);
    releOFF();
    delay(settings.trigger);
  } else {
    releON();
    delay(settings.trigger);
    releOFF();
    delay(settings.mirror);
    releON();
    delay(settings.trigger);
    releOFF();
  }
}

// Function for step testing
void testStep() {
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(F("Moving 1 step"));
  lcd.setCursor(1, 1);
  lcd.print(F("forward"));
  for (int i = 0; i < settings.DOF; i++) {
    goFront();
  }
  delay(1000);
  motorStop();

  delay(1000);

  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(F("Moving 1 step"));
  lcd.setCursor(1, 1);
  lcd.print(F("backward"));
  for (int i = 0; i < settings.DOF; i++) {
    goBack();
  }
  delay(1000);
  motorStop();
}

// Function for setting steps
void setStepping() {
  digitalWrite(msg1, SET_STEP1);
  digitalWrite(msg2, SET_STEP2);
  digitalWrite(msg3, SET_STEP3);
}

// Function for backward movement
void goBack() {
  digitalWrite(enable, LOW);
  digitalWrite(msg1, SET_STEP1);
  digitalWrite(msg2, SET_STEP2);
  digitalWrite(msg3, SET_STEP3);

  digitalWrite(dirPin, HIGH);
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(settings.motorDelay);
  digitalWrite(stepPin, LOW);
  delayMicroseconds(settings.motorDelay);
  status.actualState++;
}

// Function for forward movement
void goFront() {
  digitalWrite(enable, LOW);
  digitalWrite(msg1, SET_STEP1);
  digitalWrite(msg2, SET_STEP2);
  digitalWrite(msg3, SET_STEP3);

  digitalWrite(dirPin, LOW);
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(settings.motorDelay);
  digitalWrite(stepPin, LOW);
  delayMicroseconds(settings.motorDelay);
}

// Function for stopping motor
void motorStop() {
  digitalWrite(enable, LOW);
  digitalWrite(msg1, LOW);
  digitalWrite(msg2, LOW);
  digitalWrite(msg3, LOW);

  digitalWrite(dirPin, LOW);
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(settings.motorDelay);
  digitalWrite(stepPin, LOW);
  delayMicroseconds(settings.motorDelay);
}

// Function for backward run
void goRunB() {
  lcd.clear();

  if (settings.DOF >= 50) {
    settings.motorDelay = 800;
  } else {
    settings.motorDelay = 300;
  }

  status.loopCounter = 0;
  status.focusCounter = 0;
  status.emergency = 0;

  switch (settings.stepsI) {
    case 0:
      settings.localDOF = ((settings.DOF / 2) * 0.75);
      status.focusMotorState = (status.motorState / settings.localDOF);
      break;
    case 1:
      settings.localDOF = (settings.DOF * 0.75);
      status.focusMotorState = (status.motorState / settings.localDOF);
      break;
  }

  settings.delayTiming = settings.timing * 1000;
  settings.delayTimingMove = settings.timingMove * 1000;
  delay(2400);  // wait before direction change

  for (int i = 0; i < status.focusMotorState; i++) {
    status.loopCounter++;

    for (int j = 0; j < settings.localDOF; j++) {
      char customKey = keypad.getKey();

      if (digitalRead(3) == LOW) {
        status.emergency = 1;
        break;
        LCDRepaint();
        delay(30);
      }
      goBack();
      status.focusCounter++;
    }

    lcd.clear();
    int remainingTime = (((settings.trigger * 2) + settings.mirror + 5000 + settings.delayTiming + settings.delayTimingMove) / 2000);
    int laps = (status.focusMotorState - status.loopCounter);

    lcd.setCursor(0, 0);
    lcd.print(status.loopCounter);
    lcd.print(F("/"));
    lcd.print(status.focusMotorState);

    lcd.setCursor(10, 0);
    lcd.print(laps * remainingTime);
    lcd.print(F("s"));

    lcd.setCursor(0, 1);
    lcd.print(F("increment "));

    lcd.setCursor(10, 1);
    lcd.print(settings.DOF);
    lcd.print(F("nm"));

    delay(settings.delayTimingMove);
    capture();
    delay(settings.delayTiming);

    if (digitalRead(3) == LOW) {
      status.emergency = 1;
      break;
      LCDRepaint();
      delay(30);
      motorStop();
    }
  }

  if ((settings.AutoReturn == 1) && (status.emergency == 0)) {
    for (int i = 0; i < status.motorState; i++) {
      goFront();
    }
    motorStop();
  }

  status.emergency = 0;
  status.focusMotorState = 0;
  status.focusCounter = 0;
  motorStop();
}

// Function for deleting trace
void delTrace() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("TRACE deleted"));
  status.actualState = 0;
  status.motorState = 0;
  delay(2500);
  LCDRepaint();
}

// Function for AutoSave
void SetAutoSave() {
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print(F("autoSAVE"));
  lcd.setCursor(2, 1);
  lcd.print(F("new values"));
  delay(1750);

  settings.autoSave = lcd.inputVal<int>("0 = OFF / 1 = ON", 0, 1, settings.autoSave, 1);
  writeIntIntoEEPROM(ADDR_MICRO_AUTOSAVE, settings.autoSave);
  delay(500);
}

// Function for setting speed
void SetSpeed() {
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print(F("MotorSpeed"));
  lcd.setCursor(0, 1);
  lcd.print(F("per single step"));
  delay(1750);

  settings.motorSpeed = lcd.inputVal<int>("Input in ms", 25, 1000, settings.motorSpeed, 25);

  if (settings.autoSave == 1) {
    writeIntIntoEEPROM(ADDR_MICRO_MOTORSPEED, settings.motorSpeed);
    delay(200);
  }
}

// Function for setting step value
void SetStepValue() {
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print(F("HiRes step"));
  lcd.setCursor(1, 1);
  lcd.print(F("movement setup"));
  delay(1750);

  settings.stepsI = lcd.inputVal<int>("0 = OFF / 1 = ON", 0, 1, settings.stepsI, 1);
  status.actualState = 0;
  status.motorState = 0;

  if (settings.autoSave == 1) {
    writeIntIntoEEPROM(ADDR_MICRO_STEPSI, settings.stepsI);
    delay(200);
  }
}

// Function for setting DOF
void SetDOF() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(settings.stepsI == 0 ? F("nm/step - normal") : F("nm/step - HiRes"));
  delay(1750);
  settings.DOF = lcd.inputVal<int>(settings.stepsI == 0 ? "Input um/step" : "Input nm/step", settings.stepsI == 0 ? 10 : 5, settings.stepsI == 0 ? 1800 : 1000, settings.DOF, settings.stepsI == 0 ? 5 : 1);

  if (settings.autoSave == 1) {
    writeIntIntoEEPROM(ADDR_MICRO_DOF, settings.DOF);
    delay(200);
  }
}

// Function for displaying Boot screen
void SetBoot() {
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(F("enable/disable"));
  lcd.setCursor(0, 1);
  lcd.print(F("init. BOOTscreen"));
  delay(1750);
  settings.bootVal = lcd.inputVal<int>("0 = OFF / 1 = ON", 0, 1, settings.bootVal);
  writeIntIntoEEPROM(ADDR_MICRO_BOOTVAL, settings.bootVal);
  delay(500);
}

// Function for setting microscope
void SetPresetM() {
  settings.presetVal = 0;

  settings.stepsI = readIntFromEEPROM(ADDR_MICRO_STEPSI);
  settings.timing = readIntFromEEPROM(ADDR_MICRO_TIMING);
  settings.DOF = readIntFromEEPROM(ADDR_MICRO_DOF);
  settings.timingMove = readIntFromEEPROM(ADDR_MICRO_TIMINGMOVE);
  settings.trigger = readIntFromEEPROM(ADDR_MICRO_TRIGGER);
  settings.mirror = readIntFromEEPROM(ADDR_MICRO_MIRROR);
  settings.ShootMode = readIntFromEEPROM(ADDR_MICRO_SHOOTMODE);
  settings.AutoReturn = readIntFromEEPROM(ADDR_MICRO_AUTORETURN);
  settings.motorSpeed = readIntFromEEPROM(ADDR_MICRO_MOTORSPEED);
  settings.added = readIntFromEEPROM(ADDR_MICRO_ADDED);
  settings.bootVal = readIntFromEEPROM(ADDR_MICRO_BOOTVAL);
  settings.autoSave = readIntFromEEPROM(ADDR_MICRO_AUTOSAVE);

  delay(100);
  writeIntIntoEEPROM(ADDR_MICRO_STEPSI, settings.stepsI);
  writeIntIntoEEPROM(ADDR_MICRO_TIMING, settings.timing);
  writeIntIntoEEPROM(ADDR_MICRO_DOF, settings.DOF);
  writeIntIntoEEPROM(ADDR_MICRO_TIMINGMOVE, settings.timingMove);
  writeIntIntoEEPROM(ADDR_MICRO_TRIGGER, settings.trigger);
  writeIntIntoEEPROM(ADDR_MICRO_MIRROR, settings.mirror);
  writeIntIntoEEPROM(ADDR_MICRO_SHOOTMODE, settings.ShootMode);
  writeIntIntoEEPROM(ADDR_MICRO_AUTORETURN, settings.AutoReturn);
  writeIntIntoEEPROM(ADDR_MICRO_MOTORSPEED, settings.motorSpeed);
  writeIntIntoEEPROM(ADDR_MICRO_ADDED, settings.added);
  writeIntIntoEEPROM(ADDR_MICRO_BOOTVAL, settings.bootVal);
  writeIntIntoEEPROM(ADDR_MICRO_PRESETVAL, settings.presetVal);
  writeIntIntoEEPROM(ADDR_MICRO_AUTOSAVE, settings.autoSave);
  delay(100);
  settings.presetVal = 0;

  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print(F("Microscope"));
  lcd.setCursor(1, 1);
  lcd.print(F("preset chosen"));
  delay(1750);

  status.actualState = 0;
  status.motorState = 0;
}

// Function for setting shooting mode
void SetShootMode() {
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(F("enable/disable"));
  lcd.setCursor(0, 1);
  lcd.print(F("mirrorLOCK func."));
  delay(1750);
  settings.ShootMode = lcd.inputVal<int>("0 = OFF / 1 = ON", 0, 1, settings.ShootMode);

  if (settings.autoSave == 1) {
    writeIntIntoEEPROM(ADDR_MICRO_SHOOTMODE, settings.ShootMode);
    delay(200);
  }
}

// Function for setting added length
void SetAdded() {
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print(F("set length"));
  lcd.setCursor(1, 1);
  lcd.print(F("added to move"));
  delay(1750);
  settings.added = lcd.inputVal<int>("Input um", 10, 100, settings.added, 10);

  if (settings.autoSave == 1) {
    writeIntIntoEEPROM(ADDR_MICRO_ADDED, settings.added);
    delay(200);
  }
}

// Function for setting automatic return
void SetAutoReturn() {
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(F("enable/disable"));
  lcd.setCursor(3, 1);
  lcd.print(F("after stack"));
  delay(1750);
  settings.AutoReturn = lcd.inputVal<int>("0 = OFF / 1 = ON", 0, 1, settings.AutoReturn);

  if (settings.autoSave == 1) {
    writeIntIntoEEPROM(ADDR_MICRO_AUTORETURN, settings.AutoReturn);
    delay(200);
  }
}

// Function for setting delay
void SetDelay() {
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(F("shutter speed"));
  lcd.setCursor(0, 1);
  lcd.print(F("per single photo"));
  delay(1750);
  settings.timing = lcd.inputVal<int>("Input delay (s)", 1, 20, settings.timing);

  if (settings.autoSave == 1) {
    writeIntIntoEEPROM(ADDR_MICRO_TIMING, settings.timing);
    delay(200);
  }
}

// Function for setting movement delay
void SetDelayMove() {
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(F("delay between"));
  lcd.setCursor(1, 1);
  lcd.print(F("move and photo"));
  delay(1750);
  settings.timingMove = lcd.inputVal<int>("Input delay (s)", 1, 20, settings.timingMove);

  if (settings.autoSave == 1) {
    writeIntIntoEEPROM(ADDR_MICRO_TIMINGMOVE, settings.timingMove);
    delay(200);
  }
}

// Function for setting trigger delay
void SetDelayTrigger() {
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(F("adjust remote"));
  lcd.setCursor(1, 1);
  lcd.print(F("trigger length"));
  delay(1750);
  settings.trigger = lcd.inputVal<int>("Input delay (us)", 250, 2000, settings.trigger, 250);

  if (settings.autoSave == 1) {
    writeIntIntoEEPROM(ADDR_MICRO_TRIGGER, settings.trigger);
    delay(200);
  }
}

// Function for setting mirror delay
void SetMirror() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("time to prevent"));
  lcd.setCursor(0, 1);
  lcd.print(F("mirror vibration"));
  delay(1750);
  settings.mirror = lcd.inputVal<int>("Input delay (us)", 250, 5000, settings.mirror, 250);

  if (settings.autoSave == 1) {
    writeIntIntoEEPROM(ADDR_MICRO_MIRROR, settings.mirror);
    delay(200);
  }
}

// Function for saving to EEPROM
void SetSave() {
  writeIntIntoEEPROM(ADDR_MICRO_STEPSI, settings.stepsI);
  writeIntIntoEEPROM(ADDR_MICRO_TIMING, settings.timing);
  writeIntIntoEEPROM(ADDR_MICRO_DOF, settings.DOF);
  writeIntIntoEEPROM(ADDR_MICRO_TIMINGMOVE, settings.timingMove);
  writeIntIntoEEPROM(ADDR_MICRO_TRIGGER, settings.trigger);
  writeIntIntoEEPROM(ADDR_MICRO_MIRROR, settings.mirror);
  writeIntIntoEEPROM(ADDR_MICRO_SHOOTMODE, settings.ShootMode);
  writeIntIntoEEPROM(ADDR_MICRO_AUTORETURN, settings.AutoReturn);
  writeIntIntoEEPROM(ADDR_MICRO_MOTORSPEED, settings.motorSpeed);
  writeIntIntoEEPROM(ADDR_MICRO_ADDED, settings.added);
  writeIntIntoEEPROM(ADDR_MICRO_BOOTVAL, settings.bootVal);
  writeIntIntoEEPROM(ADDR_MICRO_PRESETVAL, settings.presetVal);
  writeIntIntoEEPROM(ADDR_MICRO_AUTOSAVE, settings.autoSave);
  writeIntIntoEEPROM(ADDR_objective1C1, settings.valueO1C1);
  writeIntIntoEEPROM(ADDR_objective2C1, settings.valueO2C1);
  writeIntIntoEEPROM(ADDR_objective3C1, settings.valueO3C1);
  writeIntIntoEEPROM(ADDR_objective4C1, settings.valueO4C1);
  writeIntIntoEEPROM(ADDR_objective5C1, settings.valueO5C1);

  writeIntIntoEEPROM(ADDR_objective1C2, settings.valueO1C2);
  writeIntIntoEEPROM(ADDR_objective2C2, settings.valueO2C2);
  writeIntIntoEEPROM(ADDR_objective3C2, settings.valueO3C2);
  writeIntIntoEEPROM(ADDR_objective4C2, settings.valueO4C2);
  writeIntIntoEEPROM(ADDR_objective5C2, settings.valueO5C2);

  writeIntIntoEEPROM(ADDR_objective1C3, settings.valueO1C3);
  writeIntIntoEEPROM(ADDR_objective2C3, settings.valueO2C3);
  writeIntIntoEEPROM(ADDR_objective3C3, settings.valueO3C3);
  writeIntIntoEEPROM(ADDR_objective4C3, settings.valueO4C3);
  writeIntIntoEEPROM(ADDR_objective5C3, settings.valueO5C3);
  delay(200);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("saved to EEPROM"));
  delay(1750);
}

// Function for loading default values for microscope
void SetDefaultsM() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Micro. loaded"));
  delay(750);

  settings.stepsI = 1;
  settings.timing = 3;
  settings.DOF = 20;
  settings.timingMove = 2;
  settings.trigger = 500;
  settings.mirror = 750;
  settings.ShootMode = 0;
  settings.AutoReturn = 1;
  settings.motorSpeed = 500;
  settings.added = 50;
  settings.bootVal = 1;
  settings.presetVal = 0;
  settings.autoSave = 0;

  settings.valueO1C1 = 700;
  settings.valueO2C1 = 550;
  settings.valueO3C1 = 280;
  settings.valueO4C1 = 180;
  settings.valueO5C1 = 110;

  settings.valueO1C2 = 280;
  settings.valueO2C2 = 110;
  settings.valueO3C2 = 45;
  settings.valueO4C2 = 30;
  settings.valueO5C2 = 20;

  settings.valueO1C3 = 180;
  settings.valueO2C3 = 90;
  settings.valueO3C3 = 45;
  settings.valueO4C3 = 30;
  settings.valueO5C3 = 20;

  delay(100);
  writeIntIntoEEPROM(ADDR_objective1C1, settings.valueO1C1);
  writeIntIntoEEPROM(ADDR_objective2C1, settings.valueO2C1);
  writeIntIntoEEPROM(ADDR_objective3C1, settings.valueO3C1);
  writeIntIntoEEPROM(ADDR_objective4C1, settings.valueO4C1);
  writeIntIntoEEPROM(ADDR_objective5C1, settings.valueO5C1);

  writeIntIntoEEPROM(ADDR_objective1C2, settings.valueO1C2);
  writeIntIntoEEPROM(ADDR_objective2C2, settings.valueO2C2);
  writeIntIntoEEPROM(ADDR_objective3C2, settings.valueO3C2);
  writeIntIntoEEPROM(ADDR_objective4C2, settings.valueO4C2);
  writeIntIntoEEPROM(ADDR_objective5C2, settings.valueO5C2);

  writeIntIntoEEPROM(ADDR_objective1C3, settings.valueO1C3);
  writeIntIntoEEPROM(ADDR_objective2C3, settings.valueO2C3);
  writeIntIntoEEPROM(ADDR_objective3C3, settings.valueO3C3);
  writeIntIntoEEPROM(ADDR_objective4C3, settings.valueO4C3);
  writeIntIntoEEPROM(ADDR_objective5C3, settings.valueO5C3);

  writeIntIntoEEPROM(ADDR_MICRO_STEPSI, settings.stepsI);
  writeIntIntoEEPROM(ADDR_MICRO_TIMING, settings.timing);
  writeIntIntoEEPROM(ADDR_MICRO_DOF, settings.DOF);
  writeIntIntoEEPROM(ADDR_MICRO_TIMINGMOVE, settings.timingMove);
  writeIntIntoEEPROM(ADDR_MICRO_TRIGGER, settings.trigger);
  writeIntIntoEEPROM(ADDR_MICRO_MIRROR, settings.mirror);
  writeIntIntoEEPROM(ADDR_MICRO_SHOOTMODE, settings.ShootMode);
  writeIntIntoEEPROM(ADDR_MICRO_AUTORETURN, settings.AutoReturn);
  writeIntIntoEEPROM(ADDR_MICRO_MOTORSPEED, settings.motorSpeed);
  writeIntIntoEEPROM(ADDR_MICRO_ADDED, settings.added);
  writeIntIntoEEPROM(ADDR_MICRO_BOOTVAL, settings.bootVal);
  writeIntIntoEEPROM(ADDR_MICRO_PRESETVAL, settings.presetVal);
  writeIntIntoEEPROM(ADDR_MICRO_AUTOSAVE, settings.autoSave);
  delay(100);
}

//------------------------------------------------------
// MENU
sMenuItem menu[] = {
  { menuBack, menuRoot, "", NULL },
  { menuBack, menuRoot, "fccFS2 PRO", NULL },

  { menuRoot, menuTest, "Test", NULL },
  { menuTest, menuShot, "testSHOT", capture },
  { menuTest, menuTestStep, "testStep", testStep },
  { menuTest, menuBack, " <---BACK----", NULL },

  { menuRoot, menuRun, "RUN stack", NULL },
  { menuRun, chooseObjective, "Presets", NULL },
  { chooseObjective, chooseCarousel1, "small", NULL },
  { chooseCarousel1, getObjective1C1_menu, "2.5x", getObjective1C1 },
  { chooseCarousel1, getObjective2C1_menu, "3.2x", getObjective2C1 },
  { chooseCarousel1, getObjective3C1_menu, "6.3x", getObjective3C1 },
  { chooseCarousel1, getObjective4C1_menu, "10x", getObjective4C1 },
  { chooseCarousel1, getObjective5C1_menu, "16x", getObjective5C1 },
  { chooseCarousel1, menuRunB, " RUN preset", goRunB },
  { chooseCarousel1, menuBack, " <---BACK----", NULL },

  { chooseObjective, chooseCarousel2, "apo", NULL },
  { chooseCarousel2, getObjective1C2_menu, "6.3x", getObjective1C2 },
  { chooseCarousel2, getObjective2C2_menu, "16x", getObjective2C2 },
  { chooseCarousel2, getObjective3C2_menu, "40x", getObjective3C2 },
  { chooseCarousel2, getObjective4C2_menu, "63x", getObjective4C2 },
  { chooseCarousel2, getObjective5C2_menu, "100x", getObjective5C2 },
  { chooseCarousel2, menuRunB, " RUN preset", goRunB },
  { chooseCarousel2, menuBack, " <---BACK----", NULL },

  { chooseObjective, chooseCarousel3, "GF", NULL },
  { chooseCarousel3, getObjective1C3_menu, "10x", getObjective1C3 },
  { chooseCarousel3, getObjective2C3_menu, "20x", getObjective2C3 },
  { chooseCarousel3, getObjective3C3_menu, "40x", getObjective3C3 },
  { chooseCarousel3, getObjective4C3_menu, "63x", getObjective4C3 },
  { chooseCarousel3, getObjective5C3_menu, "100x", getObjective5C3 },
  { chooseCarousel3, menuRunB, " RUN preset", goRunB },
  { chooseCarousel3, menuBack, " <---BACK----", NULL },

  { chooseObjective, menuBack, " <---BACK----", NULL },

  { menuRun, menuDOF, "Custom Steps", SetDOF },
  { menuRun, menuRunB, "RUN Custom", goRunB },
  { menuRun, menuNumPhotos, "Photos", SetNumPhotos },                    // New menu item for setting number of photos
  { menuRun, menuRunByPhotoCount, "RUN by Photos", goRunByPhotoCount },  // New menu option
  { menuRun, menuDelete, " Delete TRACE", delTrace },
  { menuRun, menuBack, " <---BACK----", NULL },

  { menuRoot, menuPreferences, "PREFERENCES", NULL },
  { menuPreferences, menuStep, " HiRes.", SetStepValue },
  { menuPreferences, menuTime, " time shutter", SetDelay },
  { menuPreferences, menuTimeMove, " time vibr.", SetDelayMove },
  { menuPreferences, menuShootMode, " mirrorLock", SetShootMode },
  { menuPreferences, menuMirror, " time (mirror)", SetMirror },
  { menuPreferences, menuSave, " SAVE", NULL },
  { menuPreferences, menuBack, " <---BACK----", NULL },

  { menuRoot, menuSetObjectives, "SET Objectives", NULL },
  { menuSetObjectives, setCarousel1, "small", NULL },
  { setCarousel1, setObjective1C1_menu, "2.5x", setObjective1C1 },
  { setCarousel1, setObjective2C1_menu, "3.2x", setObjective2C1 },
  { setCarousel1, setObjective3C1_menu, "6.3x", setObjective3C1 },
  { setCarousel1, setObjective4C1_menu, "10x", setObjective4C1 },
  { setCarousel1, setObjective5C1_menu, "16x", setObjective5C1 },
  { setCarousel1, menuBack, " <---BACK----", NULL },

  { menuSetObjectives, setCarousel2, "apo", NULL },
  { setCarousel2, setObjective1C2_menu, "6.3x", setObjective1C2 },
  { setCarousel2, setObjective2C2_menu, "16x", setObjective2C2 },
  { setCarousel2, setObjective3C2_menu, "40x", setObjective3C2 },
  { setCarousel2, setObjective4C2_menu, "63x", setObjective4C2 },
  { setCarousel2, setObjective5C2_menu, "100x", setObjective5C2 },
  { setCarousel2, menuBack, " <---BACK----", NULL },

  { menuSetObjectives, setCarousel3, "GF", NULL },
  { setCarousel3, setObjective1C3_menu, "10x", setObjective1C3 },
  { setCarousel3, setObjective2C3_menu, "20x", setObjective2C3 },
  { setCarousel3, setObjective3C3_menu, "40x", setObjective3C3 },
  { setCarousel3, setObjective4C3_menu, "63x", setObjective4C3 },
  { setCarousel3, setObjective5C3_menu, "100x", setObjective5C3 },
  { setCarousel3, menuBack, " <---BACK----", NULL },

  { menuSetObjectives, menuBack, " <---BACK----", NULL },

  { menuRoot, menuSettings, "SETTINGS", NULL },
  { menuSettings, menuAutoReturn, " AutoReturn", SetAutoReturn },
  { menuSettings, menuTimeTrigger, " time (trigg)", SetDelayTrigger },
  { menuSettings, menuSpeed, " motor speed", SetSpeed },
  { menuSettings, menuAdded, " added length", SetAdded },
  { menuSettings, menuBoot, " BOOT screen", SetBoot },
  { menuSettings, menuAutoSave, " AutoSAVE", SetAutoSave },
  { menuSettings, menuBack, " <-BACK-", NULL },

  { menuRoot, menuSave_Restore, "SAVE/RESTORE", NULL },
  { menuSave_Restore, menuSave, " SAVE", SetSave },
  { menuSave_Restore, menuRestore, " LOAD DEFAULTS  ", SetDefaultsM },
  { menuSave_Restore, menuBack, " <---BACK----", NULL },

  { menuRoot, menuInfo, "VERSION", NULL },
  { menuInfo, menuInfoDisp, "version 1.2.0.", NULL },
  { menuInfo, menuInfoDisp, "Aug. 2024", NULL },
  { menuInfo, menuBack, " <---BACK----", NULL },

  { menuRoot, menuBack, " <---HOME----", NULL },
};
uint8_t menuLen = sizeof(menu) / sizeof(sMenuItem);

// Function for redrawing LCD
void LCDRepaint() {
  if (firstRun == 0) {
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print(settings.autoSave == 0 ? F("auto SAVE OFF") : F("auto SAVE ON"));
    lcd.setCursor(1, 1);
    lcd.print(settings.AutoReturn == 0 ? F("auto RETURN OFF") : F("auto RETURN ON"));
    delay(4000);
    firstRun = 1;
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(String("nm ") + String(settings.DOF));
  lcd.setCursor(8, 0);
  lcd.print(settings.ShootMode == 0 ? F("N") : F("ML"));
  lcd.setCursor(12, 0);
  lcd.print(settings.stepsI == 0 ? F("norm") : F("hRes"));
  lcd.setCursor(0, 1);
  lcd.print(String("exp ") + String(settings.timing) + String("s"));
  lcd.setCursor(7, 1);
  lcd.print(String(" move ") + String(settings.timingMove) + String("s"));
}

// Setup function
void setup() {
  lcd.begin();
  lcd.attachEncoder(pinCLK, pinDT, pinSW);
  delay(500);
  settings.presetVal = readIntFromEEPROM(ADDR_MICRO_PRESETVAL);  // Load default values for microscope

  delay(500);

  // Load default values from EEPROM
  settings.stepsI = readIntFromEEPROM(ADDR_MICRO_STEPSI);
  settings.timing = readIntFromEEPROM(ADDR_MICRO_TIMING);
  settings.DOF = readIntFromEEPROM(ADDR_MICRO_DOF);
  settings.timingMove = readIntFromEEPROM(ADDR_MICRO_TIMINGMOVE);
  settings.trigger = readIntFromEEPROM(ADDR_MICRO_TRIGGER);
  settings.mirror = readIntFromEEPROM(ADDR_MICRO_MIRROR);
  settings.ShootMode = readIntFromEEPROM(ADDR_MICRO_SHOOTMODE);
  settings.AutoReturn = readIntFromEEPROM(ADDR_MICRO_AUTORETURN);
  settings.motorSpeed = readIntFromEEPROM(ADDR_MICRO_MOTORSPEED);
  settings.added = readIntFromEEPROM(ADDR_MICRO_ADDED);
  settings.bootVal = readIntFromEEPROM(ADDR_MICRO_BOOTVAL);
  settings.autoSave = readIntFromEEPROM(ADDR_MICRO_AUTOSAVE);
  settings.numPhotos = readIntFromEEPROM(ADDR_MICRO_NUMPHOTOS);  // Load number of photos from EEPROM

  settings.valueO1C1 = readIntFromEEPROM(ADDR_objective1C1);
  settings.valueO2C1 = readIntFromEEPROM(ADDR_objective2C1);
  settings.valueO3C1 = readIntFromEEPROM(ADDR_objective3C1);
  settings.valueO4C1 = readIntFromEEPROM(ADDR_objective4C1);
  settings.valueO5C1 = readIntFromEEPROM(ADDR_objective5C1);

  settings.valueO1C2 = readIntFromEEPROM(ADDR_objective1C2);
  settings.valueO2C2 = readIntFromEEPROM(ADDR_objective2C2);
  settings.valueO3C2 = readIntFromEEPROM(ADDR_objective3C2);
  settings.valueO4C2 = readIntFromEEPROM(ADDR_objective4C2);
  settings.valueO5C2 = readIntFromEEPROM(ADDR_objective5C2);

  settings.valueO1C3 = readIntFromEEPROM(ADDR_objective1C3);
  settings.valueO2C3 = readIntFromEEPROM(ADDR_objective2C3);
  settings.valueO3C3 = readIntFromEEPROM(ADDR_objective3C3);
  settings.valueO4C3 = readIntFromEEPROM(ADDR_objective4C3);
  settings.valueO5C3 = readIntFromEEPROM(ADDR_objective5C3);

  delay(200);
  settings.presetVal = 0;

  delay(200);

  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(enable, OUTPUT);
  pinMode(msg1, OUTPUT);
  pinMode(msg2, OUTPUT);
  pinMode(msg3, OUTPUT);
  pinMode(releTrig, OUTPUT);

  if (settings.bootVal == 1) {
    lcd.setCursor(3, 0);
    lcd.print(F("fccFS2 PRO"));
    for (int letter = 0; letter <= strlen(messagePadded) - 16; letter++) {
      showLetters(0, letter);
    }
  } else {
    delay(300);
  }

  lcd.clear();
  LCDRepaint();
  status.focusCounter = 1;  // Initialization to prevent division by zero

  releON();
  delay(500);
  releOFF();
  delay(100);
}

// Loop function
void loop() {
  digitalWrite(enable, LOW);
  digitalWrite(msg1, LOW);
  digitalWrite(msg2, LOW);
  digitalWrite(msg3, LOW);

  char customKey = keypad.getKey();

  switch (settings.stepsI) {
    case 0:
      stepSettings = 0x03;  // setStep1 = 1, setStep2 = 1, setStep3 = 0
      settings.newStepsI = 0;
      break;
    case 1:
      stepSettings = 0x07;  // setStep1 = 1, setStep2 = 1, setStep3 = 1
      settings.newStepsI = 1;
      break;
  }

  if (customKey == '1') {
    settings.motorDelay = (settings.stepsI == 0) ? settings.motorSpeed * 2 : settings.motorSpeed;
    while (true) {
      goFront();
      if (status.actualState > 0) status.actualState--;
      if (keypad.getKey() == '1') break;
    }
    LCDRepaint();
    delay(30);
  }

  if (customKey == '2') {
    status.motorState = 0;
    delay(10);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("START POSITION"));
    lcd.setCursor(0, 1);
    lcd.print(F("SAVED"));
    delay(500);
    status.actualState = 0;
    LCDRepaint();
  }

  if (customKey == '3') {
    settings.motorDelay = (settings.stepsI == 0) ? settings.motorSpeed * 2 : settings.motorSpeed;
    while (true) {
      goBack();
      if (keypad.getKey() == '3') break;
    }
    LCDRepaint();
    delay(30);
  }

  if (customKey == '4') {
    settings.motorDelay = settings.motorSpeed;
    status.motorState = (status.actualState + settings.added / 2);

    delay(10);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("END POSITION"));
    lcd.setCursor(0, 1);
    lcd.print(F("SAVED"));
    delay(500);
    LCDRepaint();
    delay(1000);

    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print(F("Moving to the"));
    lcd.setCursor(1, 1);
    lcd.print(F("START position"));
    delay(400);

    for (int i = 0; i < status.motorState; i++) {
      goFront();
    }
    status.motorState = (status.motorState + settings.added);
    delay(300);
    LCDRepaint();
  }

  if (lcd.getEncoderState() == eButton) {
    lcd.showMenu(menu, menuLen, 1);
    LCDRepaint();
  }
}