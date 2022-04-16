#ifndef FCCFS-PRO_1.0.0_H
#define FCCFS-PRO_1.0.0_H

#include <Arduino.h>

void showLetters(int printStart, int startLetter);
void releON();
void releOFF();
void capture();
void setStepping();
void goFront();
void goBack();
void motorStop();
void goF();
void goB();
void goRunB();
void goRunF();
int getItemIndexByKey(uint8_t key);
void LCDRepaint();
void delTrace();
void SetStepValue();
void SetDOF();
void SetDelay();
void SetSave();
void SetDefaults();
void setup();
void loop();

#endif