#ifndef FCCFS-PRO_1.2.0_H
#define FCCFS-PRO_1.2.0_H

#include <Arduino.h>

void writeIntIntoEEPROM(int address, int number);
int readIntFromEEPROM(int address);
void showLetters(int printStart, int startLetter);
void releON();
void releOFF();
void setObjective1C1();
void getObjective1C1();
void setObjective2C1();
void getObjective2C1();
void setObjective3C1();
void getObjective3C1();
void setObjective4C1();
void getObjective4C1();
void setObjective5C1();
void getObjective5C1();
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
void SetNumPhotos();
void goRunByPhotoCount();
void capture();
void testStep();
void setStepping();
void goBack();
void goFront();
void motorStop();
void goRunB();
void delTrace();
void SetAutoSave();
void SetSpeed();
void SetStepValue();
void SetDOF();
void SetBoot();
void SetPresetM();
void SetShootMode();
void SetAdded();
void SetAutoReturn();
void SetDelay();
void SetDelayMove();
void SetDelayTrigger();
void SetMirror();
void SetSave();
void SetDefaultsM();
void LCDRepaint();
void setup();
void loop();

#endif