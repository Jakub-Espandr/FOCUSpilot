# `EEPROMTyped` Library for Arduino ![Version 1.0.0](https://img.shields.io/badge/Version-1.0.0-blue.svg)

**Written by:** *Terence F. Golla*.  
**Copyright:** **2021**, *Terence F. Golla, Released under [MIT](/LICENSE) licence*.

## About

The EEPROM Typed Library is a lightweight static library for reading and writing standard Arduino data types to the EEPROM. 

The library provides a write and read method call that takes a memory address and standard data type variable which is deconstructed into or reconstructed from byte values written or read from the EEPROM memory.  The library also provides a sizeof used to return the variable size in bytes which can be used to calculate memory addresses.  The library works with array, bool, byte, char, double, float, int, long, short, size_t, string, unsigned char, unsigned int, unsigned long and word data types.

## Usage

### `write(address, variable)`
The write method takes the memory address where it begins to write the value of the variable broken into bytes. The memory usage depends on the sizeof the variable data type.

```
int blinkFrequency = 5500;

EEPROMTyped.write(0, blinkFrequency);
```
### `read(address, variable)`
The read method takes the memory address where it begins to read the value of the variable broken into bytes. The memory usage depends on the sizeof the variable data type.

```
int blinkFrequency;

EEPROMTyped.read(0, blinkFrequency);

Serial.print("The blink frequency is ");
Serial.println(blinkFrequency);
```
### `sizeOf(variable)`
The sizeOf method returns the size of the variable's data type in bytes. The method is provided to assist in calculating memory addresses.

```
char message[] = "Hello World!";
int blinkFrequency;

byte messageEEPROMAddress = 0;
byte blinkFrequencyEEPROMAddress = messageEEPROMAddress + EEPROMTyped.sizeOf(message);

EEPROMTyped.write(messageEEPROMAddress, message);
EEPROMTyped.write(blinkFrequencyEEPROMAddress, blinkFrequency);

char messageRead[13];
int blinkFrequencyRead;

EEPROMTyped.write(messageEEPROMAddress, messageRead);
EEPROMTyped.write(blinkFrequencyEEPROMAddress, blinkFrequencyRead);

Serial.print("The message in memory is... ");
Serial.println(messageRead);

Serial.print("The blink frequency in memory is ");
Serial.println(blinkFrequency);
```