#include <EEPROMTyped.h>

// Global variables used to demistrate writting different data types to memory.
bool boolVariable;
byte byteVariable;
char charVariable;
double doubleVariable;
float floatVariable;
int integerVariable;
int integerArray[6];
long longVariable;
short shortVariable;
size_t size_tVariable;
char stringVariable[13];
unsigned char unsignedCharVariable;
unsigned int unsignedIntVariable;
unsigned long unsignedLongVariable;
word wordVariable;

// Memory pointers
byte boolVariableEEPROMAddress = 0;
byte byteVariableEEPROMAddress = boolVariableEEPROMAddress + EEPROMTyped.sizeOf(boolVariable);
byte charVariableEEPROMAddress = byteVariableEEPROMAddress + EEPROMTyped.sizeOf(byteVariable);
byte doubleVariableEEPROMAddress = charVariableEEPROMAddress + EEPROMTyped.sizeOf(charVariable);
byte floatVariableEEPROMAddress = doubleVariableEEPROMAddress + EEPROMTyped.sizeOf(doubleVariable);
byte integerVariableEEPROMAddress = floatVariableEEPROMAddress + EEPROMTyped.sizeOf(floatVariable);
byte integerArrayEEPROMAddress = integerVariableEEPROMAddress + EEPROMTyped.sizeOf(integerVariable);
byte longVariableEEPROMAddress = integerArrayEEPROMAddress + EEPROMTyped.sizeOf(integerArray);
byte shortVariableEEPROMAddress = longVariableEEPROMAddress + EEPROMTyped.sizeOf(longVariable);
byte size_tVariableEEPROMAddress = shortVariableEEPROMAddress + EEPROMTyped.sizeOf(shortVariable);
byte stringVariableEEPROMAddress = size_tVariableEEPROMAddress + EEPROMTyped.sizeOf(size_tVariable);
byte unsignedCharVariableEEPROMAddress = stringVariableEEPROMAddress + EEPROMTyped.sizeOf(stringVariable);
byte unsignedIntVariableEEPROMAddress = unsignedCharVariableEEPROMAddress + EEPROMTyped.sizeOf(unsignedCharVariable);
byte unsignedLongVariableEEPROMAddress = unsignedIntVariableEEPROMAddress + EEPROMTyped.sizeOf(unsignedIntVariable);
byte wordVariableEEPROMAddress = unsignedLongVariableEEPROMAddress + EEPROMTyped.sizeOf(unsignedLongVariable);

void setup()
{
  Serial.begin(115200);

  Serial.println("1-Set Variables, 2-Clear Variables, 3-Write Memory, 4-Read Memory");
  Serial.println();
}

void loop()
{
  if (Serial.available() > 0)
  {
    char c = Serial.read();

    if (c >= '1' && c <= '4')
    {
      switch (c)
      {
      case '1':
        setVariables();
        Serial.println("Set Variables");
        Serial.println();
        break;
      case '2':
        clearVariables();
        Serial.println("Clear Variables");
        Serial.println();
        break;
      case '3':
        writeMemory();
        Serial.println("Write Memory");
        Serial.println();
        break;
      case '4':
        readMemory();
        Serial.println("Read Memory");
        Serial.println();
        break;
      }

      printVariables();
    }
  }
}

void setVariables()
{
  boolVariable = true;
  byteVariable = 129;
  charVariable = 't';
  doubleVariable = 987654;
  floatVariable = 987654321;
  integerVariable = -4321;
  integerArray[0] = 5;
  integerArray[1] = 4;
  integerArray[2] = 3;
  integerArray[3] = 2;
  integerArray[4] = 1;
  integerArray[5] = 0;
  longVariable = 99999;
  shortVariable = 11111;
  size_tVariable = 2;
  strncpy(stringVariable, "Hello World!", 13);
  unsignedCharVariable = 'g';
  unsignedIntVariable = 44444;
  unsignedLongVariable = 88888;
  wordVariable = 12345;
}

void clearVariables()
{
  boolVariable = false;
  byteVariable = 0;
  charVariable = ' ';
  doubleVariable = 0;
  floatVariable = 0;
  integerVariable = 0;
  integerArray[0] = 0;
  integerArray[1] = 0;
  integerArray[2] = 0;
  integerArray[3] = 0;
  integerArray[4] = 0;
  integerArray[5] = 0;
  longVariable = 0;
  shortVariable = 0;
  size_tVariable = 0;
  strncpy(stringVariable, "", 1);
  unsignedCharVariable = ' ';
  unsignedIntVariable = 0;
  unsignedLongVariable = 0;
  wordVariable = 0;
}

void printVariables()
{
  Serial.print("bool: ");
  Serial.print(boolVariable);
  Serial.print("  sizeOf: ");
  Serial.println(EEPROMTyped.sizeOf(boolVariable));

  Serial.print("byte: ");
  Serial.print(byteVariable);
  Serial.print("  sizeOf: ");
  Serial.println(EEPROMTyped.sizeOf(byteVariable));

  Serial.print("char: ");
  Serial.print(charVariable);
  Serial.print("  sizeOf: ");
  Serial.println(EEPROMTyped.sizeOf(charVariable));

  Serial.print("double: ");
  Serial.print(doubleVariable);
  Serial.print("  sizeOf: ");
  Serial.println(EEPROMTyped.sizeOf(doubleVariable));

  Serial.print("float: ");
  Serial.print(floatVariable);
  Serial.print("  sizeOf: ");
  Serial.println(EEPROMTyped.sizeOf(floatVariable));

  Serial.print("integer: ");
  Serial.print(integerVariable);
  Serial.print("  sizeOf: ");
  Serial.println(EEPROMTyped.sizeOf(integerVariable));

  Serial.print("Integer Array: ");
  Serial.print(integerArray[0]);
  Serial.print(", ");
  Serial.print(integerArray[1]);
  Serial.print(", ");
  Serial.print(integerArray[2]);
  Serial.print(", ");
  Serial.print(integerArray[3]);
  Serial.print(", ");
  Serial.print(integerArray[4]);
  Serial.print(", ");
  Serial.print(integerArray[5]);
  Serial.print("  sizeOf: ");
  Serial.println(EEPROMTyped.sizeOf(integerArray));

  Serial.print("long: ");
  Serial.print(longVariable);
  Serial.print("  sizeOf: ");
  Serial.println(EEPROMTyped.sizeOf(longVariable));

  Serial.print("short: ");
  Serial.print(shortVariable);
  Serial.print("  sizeOf: ");
  Serial.println(EEPROMTyped.sizeOf(shortVariable));

  Serial.print("size_t: ");
  Serial.print(size_tVariable);
  Serial.print("  sizeOf: ");
  Serial.println(EEPROMTyped.sizeOf(size_tVariable));

  Serial.print("string: ");
  Serial.print(stringVariable);
  Serial.print("  sizeOf: ");
  Serial.println(EEPROMTyped.sizeOf(stringVariable));

  Serial.print("unsigned char: ");
  Serial.print(unsignedCharVariable);
  Serial.print("  sizeOf: ");
  Serial.println(EEPROMTyped.sizeOf(unsignedCharVariable));

  Serial.print("unsigned integer: ");
  Serial.print(unsignedIntVariable);
  Serial.print("  sizeOf: ");
  Serial.println(EEPROMTyped.sizeOf(unsignedIntVariable));

  Serial.print("unsigned long: ");
  Serial.print(unsignedLongVariable);
  Serial.print("  sizeOf: ");
  Serial.println(EEPROMTyped.sizeOf(unsignedLongVariable));

  Serial.print("word: ");
  Serial.print(wordVariable);
  Serial.print("  sizeOf: ");
  Serial.println(EEPROMTyped.sizeOf(wordVariable));

  Serial.println();
}

void writeMemory()
{
  EEPROMTyped.write(boolVariableEEPROMAddress, boolVariable);
  EEPROMTyped.write(byteVariableEEPROMAddress, byteVariable);
  EEPROMTyped.write(charVariableEEPROMAddress, charVariable);
  EEPROMTyped.write(doubleVariableEEPROMAddress, doubleVariable);
  EEPROMTyped.write(floatVariableEEPROMAddress, floatVariable);
  EEPROMTyped.write(integerVariableEEPROMAddress, integerVariable);
  EEPROMTyped.write(integerArrayEEPROMAddress, integerArray);
  EEPROMTyped.write(longVariableEEPROMAddress, longVariable);
  EEPROMTyped.write(shortVariableEEPROMAddress, shortVariable);
  EEPROMTyped.write(size_tVariableEEPROMAddress, size_tVariable);
  EEPROMTyped.write(stringVariableEEPROMAddress, stringVariable);
  EEPROMTyped.write(unsignedCharVariableEEPROMAddress, unsignedCharVariable);
  EEPROMTyped.write(unsignedIntVariableEEPROMAddress, unsignedIntVariable);
  EEPROMTyped.write(unsignedLongVariableEEPROMAddress, unsignedLongVariable);
  EEPROMTyped.write(wordVariableEEPROMAddress, wordVariable);
}

void readMemory()
{
  EEPROMTyped.read(boolVariableEEPROMAddress, boolVariable);
  EEPROMTyped.read(byteVariableEEPROMAddress, byteVariable);
  EEPROMTyped.read(charVariableEEPROMAddress, charVariable);
  EEPROMTyped.read(doubleVariableEEPROMAddress, doubleVariable);
  EEPROMTyped.read(floatVariableEEPROMAddress, floatVariable);
  EEPROMTyped.read(integerVariableEEPROMAddress, integerVariable);
  EEPROMTyped.read(integerArrayEEPROMAddress, integerArray);
  EEPROMTyped.read(longVariableEEPROMAddress, longVariable);
  EEPROMTyped.read(shortVariableEEPROMAddress, shortVariable);
  EEPROMTyped.read(size_tVariableEEPROMAddress, size_tVariable);
  EEPROMTyped.read(stringVariableEEPROMAddress, stringVariable);
  EEPROMTyped.read(unsignedCharVariableEEPROMAddress, unsignedCharVariable);
  EEPROMTyped.read(unsignedIntVariableEEPROMAddress, unsignedIntVariable);
  EEPROMTyped.read(unsignedLongVariableEEPROMAddress, unsignedLongVariable);
  EEPROMTyped.read(wordVariableEEPROMAddress, wordVariable);
}
