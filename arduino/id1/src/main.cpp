#include <Arduino.h>
#include <fastIO.h>
#include <TimerOne.h> 

// Конфигурация строк
const uint16_t dcPins[5] = {A0, A1, A2, A3, A4};

enum pins {
  Start,
  DO,
  Clk,
  Strn,
  cnt
};

const uint16_t ctlPins[pins::cnt] = {2, 3, 4, 5};

// Буфер дисплея (10x21)
uint16_t displayBuffer[21] = {0};
const uint16_t displayBufferCount = sizeof(displayBuffer) / sizeof(displayBuffer[0]);

// Установка активной строки (0-19)
void setRow(byte row, uint16_t mask) {
  // Актуализация столбцов
  for (int i = 0; i < 10; i++, mask >>= 1)
  {
    fastDigitalWrite(ctlPins[pins::Clk], HIGH);
    fastDigitalWrite(ctlPins[pins::DO], mask & 1);
    fastDigitalWrite(ctlPins[pins::Clk], LOW);
  }
  fastDigitalWrite(ctlPins[pins::Clk], HIGH);
  fastDigitalWrite(ctlPins[pins::Start], HIGH);
  fastDigitalWrite(ctlPins[pins::Strn], HIGH);
  // Декодер строки
  for (auto pin : dcPins)
  {
    fastDigitalWrite(pin, row & 1);
    row >>= 1;
  }
  fastDigitalWrite(ctlPins[pins::Strn], LOW);
  fastDigitalWrite(ctlPins[pins::Start], LOW);
  fastDigitalWrite(ctlPins[pins::Strn], HIGH);
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

void setup() {
  // Настройка пинов
  for (auto pin : dcPins) {
    fastPinMode(pin, OUTPUT);
    fastDigitalWrite(pin, LOW);
  }
  for (auto pin : ctlPins) {
    fastPinMode(pin, OUTPUT);
    fastDigitalWrite(pin, HIGH);
  }

  // Настройка таймера для динамической индикации
  Timer1.initialize(1000); // 1000Hz (1000 мкс)
  Timer1.attachInterrupt(updateDisplay);

  Serial.begin(9600);
  Serial.println("init complete");
}

void updateDisplayContent()
{
  static uint16_t fill = 0x5555;
  for(int i = 21; i >= 0; displayBuffer[i--] = (i & 1) ? fill : ~fill);
  fill = ~fill;
}

void loop() {
  // Обновляем содержимое буфера
  updateDisplayContent();
  delay(1000);
}
