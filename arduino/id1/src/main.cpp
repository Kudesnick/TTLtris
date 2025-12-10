#include <Arduino.h>
#include <GyverIO.h>
#include <STM32TimerInterrupt.h>

// Конфигурация строк
const uint16_t dcPins[5] = {PA9, PB14, PB13, PB8, PB5};

enum pins {
  Start,
  DO,
  Clk,
  Strn,
  cnt
};

const uint16_t ctlPins[pins::cnt] = {PB6, PC13, PB9, PB7};

// Буфер дисплея (10x21)
uint16_t displayBuffer[21] = {0};
const uint16_t displayBufferCount = sizeof(displayBuffer) / sizeof(displayBuffer[0]);

// Установка активной строки (0-20)
void setRow(byte row, uint16_t mask) {
  // Актуализация столбцов
  for (int i = 0; i < 10; i++, mask >>= 1)
  {
    digitalWrite(ctlPins[pins::Clk], HIGH);
    digitalWrite(ctlPins[pins::DO], mask & 1);
    digitalWrite(ctlPins[pins::Clk], LOW);
  }
  digitalWrite(ctlPins[pins::Clk], HIGH);
  digitalWrite(ctlPins[pins::Start], HIGH);
  digitalWrite(ctlPins[pins::Strn], HIGH);
  // Декодер строки
  for (auto pin : dcPins)
  {
    digitalWrite(pin, row & 1);
    row >>= 1;
  }
  digitalWrite(ctlPins[pins::Strn], LOW);
  digitalWrite(ctlPins[pins::Start], LOW);
  digitalWrite(ctlPins[pins::Strn], HIGH);
}

// Прерывание для обновления дисплея
void updateDisplay() {
  static uint16_t currentRow = 0;
  setRow(currentRow, displayBuffer[currentRow]);
  if (++currentRow >= displayBufferCount)
  {
    currentRow = 0;
  }
}

STM32Timer ITimer(TIM1);

void setup() {
  // Настройка пинов
  for (auto pin : dcPins) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }
  for (auto pin : ctlPins) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
  }

  // Настройка таймера для динамической индикации
  ITimer.attachInterruptInterval(1000, updateDisplay);

  Serial.begin(9600);
  Serial.println("init complete");
}

void updateDisplayContent()
{
  static uint16_t fill = 0x5555;
  for(int i = 21; i >= 0; displayBuffer[i--] = (i & 1) ? fill : ~fill);
  fill = ~fill;
//  memset(displayBuffer, 0xFF, sizeof(displayBuffer));
}

void loop() {
  // Обновляем содержимое буфера
  updateDisplayContent();
  delay(1000);
}
