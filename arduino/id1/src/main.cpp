#include <Arduino.h>
#include <GyverIO.h>
#include <STM32TimerInterrupt.h>

const uint32_t SCAN_TICK = 1;
const uint32_t FRAME_TICK = 1000;
const uint32_t KEY_TICK = 25;

// Конфигурация строк
const uint16_t dcPins[5] = {PB5, PB6, PB7, PB8, PB9};

enum pins {
  Start,
  DO,
  Clk,
  Strn,
  cnt
};

const uint16_t ctlPins[pins::cnt] = {PB15, PB14, PB12, PB13};

const uint16_t ledPin = PC13;
const uint16_t keyPin = PA0;

// Буфер дисплея (10x21)
uint16_t displayBuffer[21] = {0};
const uint16_t displayBufferCount = sizeof(displayBuffer) / sizeof(displayBuffer[0]);

STM32Timer ITimerDisplay(TIM1);
STM32Timer ITimerLed(TIM2);

bool traceMode = false;

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

  digitalWrite(ledPin, currentRow & 1);
}

// Прерывание для обработки светодиода и кнопки
void updateLed() {
  static uint16_t key = 0;
  static uint8_t led = 0;

  if (!digitalRead(keyPin))
  {
    if (key < 255) key++;
    if (key == 50)
    {
      // long push
      if (traceMode){
        ITimerDisplay.enableTimer();

      }
      else{
        ITimerDisplay.disableTimer();
      }
      traceMode = !traceMode; 
    }
  }
  else
  {
    if (key > 2 && key < 50)
    {
      // push up
      if (traceMode)
      {
        updateDisplay();
      }
    }
    key = 0;
  }
}

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

  pinMode(ledPin, OUTPUT);
  pinMode(keyPin, INPUT_PULLUP);

  // Настройка таймеров
  ITimerDisplay.attachInterruptInterval(SCAN_TICK * 1000, updateDisplay);
  ITimerLed.attachInterruptInterval(KEY_TICK * 1000, updateLed);
}

void updateDisplayContent()
{
  static auto step = 0;
  switch (step){
    case 0:
    case 1:
      static uint16_t fill = 0x5555;
      for(int i = 21; i >= 0; displayBuffer[i--] = (i & 1) ? fill : ~fill);
      fill = ~fill;
      step++;
      break;
    default:
      memset(displayBuffer, 0xFF, sizeof(displayBuffer));
      step = 0;
      break;
  }
}

void loop() {
  // Обновляем содержимое буфера
  if (!traceMode) updateDisplayContent();
  delay(FRAME_TICK);
}
