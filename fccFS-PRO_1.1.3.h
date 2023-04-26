#ifndef FCCFS-PRO_1.1.3_H
#define FCCFS-PRO_1.1.3_H

#include <Arduino.h>

void writeIntIntoEEPROM(int address, int number);
int readIntFromEEPROM(int address);
void showLetters(int printStart, int startLetter);
void releON();
void releOFF();
void capture();
void setStepping();
void goBack();
void goBack2();
void goFront();
void goFront2();
void motorStop();
void goB();
void goF();
void goRunF();
void goRunB();
int getItemIndexByKey(uint8_t key);
void LCDRepaint();
void delTrace();
void SetSpeed();
void SetStepValue();
void SetDOF();
void SetModeFS();
void SetModeM();
void SetDOF();
void SetBoot();
void SetPreset();
void SetShootMode();
void SetAdded();
void SetAutoReturn();
void SetDelay();
void SetDelayMove();
void SetDelayTrigger();
void SetMirror();
void SetSave();
void SetSaveFS();
void SetSaveM();
void SetDefaultsFS();
void SetDefaultsM();
void setup();
void loop();

#endif