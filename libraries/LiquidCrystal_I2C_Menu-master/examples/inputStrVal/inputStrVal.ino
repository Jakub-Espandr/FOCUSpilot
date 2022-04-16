/* Пример использования функции inputStrVal. Синтаксис:
 * inputStrVal(title, buffer, len, availSymbols), где
 * title - заголовок
 * buffer - указатель на массив char для вводимых символов
 * len - длина вводимого значения
 * availSymbols - массив символов, доступных для ввода и редактирования
 */
 
#include <Wire.h>
#include <LiquidCrystal_I2C_Menu.h>
LiquidCrystal_I2C_Menu lcd(0x27, 20, 4);

// Пины, к которым подключен энкодер
#define pinCLK 2
#define pinDT  3
#define pinSW  4

char *buffer; // Буфер для ввода

void setup() {
  lcd.begin();
  lcd.attachEncoder(pinDT, pinCLK, pinSW);
  buffer = (char*) malloc (16); // Выделим 16 байт (15 символов + конец строки)
  memset(buffer, '\0', 16); // Заполним их все символом конца строки
}

void loop() {
  if (lcd.inputStrVal("Input your name", buffer, 15, "ABCDEFGHIJKLMNOPQRSTUVWXYZ")){
    lcd.print("Hello,");
    lcd.printAt(0, 1, buffer);
  }
  else
    lcd.print("Input canceled");
  while (lcd.getEncoderState() == eNone);
}
