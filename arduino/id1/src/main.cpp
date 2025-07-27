#include <Arduino.h>
#include <fastIO.h>
#include <TimerOne.h> 

// Конфигурация декодеров строк
#define ROW_DECODER_BIT0 (A0)
#define ROW_DECODER_BIT1 (A1)
#define ROW_DECODER_BIT2 (A2)
#define ROW_DECODER_EN1  (A3)
#define ROW_DECODER_EN2  (A4)
#define ROW_DECODER_EN3  (A5)

// Конфигурация столбцов (10 пинов)
const byte colPins[10] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

// Буфер дисплея (10x20)
uint16_t displayBuffer[20] = {0};
const uint16_t displayBufferCount = sizeof(displayBuffer) / sizeof(displayBuffer[0]);

// Установка активной строки (0-19)
void setRow(byte row, uint16_t mask) {
  fastDigitalWrite(ROW_DECODER_EN1, HIGH);
  fastDigitalWrite(ROW_DECODER_EN2, HIGH);
  fastDigitalWrite(ROW_DECODER_EN3, HIGH);
  fastDigitalWrite(ROW_DECODER_BIT0, row & 0x01);
  fastDigitalWrite(ROW_DECODER_BIT1, (row >> 1) & 0x01);
  fastDigitalWrite(ROW_DECODER_BIT2, (row >> 2) & 0x01);
  // Актуализация столбцов
  for (auto pin : colPins)
  {
    fastDigitalWrite(pin, mask & 1);
    mask >>= 1;
  }
  // Первый декодер (строки 0-7)
  if (row < 8) {
    fastDigitalWrite(ROW_DECODER_EN1, LOW);
  } 
  // Второй декодер (строки 8-15)
  else if (row < 16) {
    fastDigitalWrite(ROW_DECODER_EN2, LOW);
  }
  // Третий декодер (строки 15-22)
  else {
    fastDigitalWrite(ROW_DECODER_EN3, LOW);
  }
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
  // Настройка пинов декодера строк
  fastPinMode(ROW_DECODER_BIT0, OUTPUT);
  fastPinMode(ROW_DECODER_BIT1, OUTPUT);
  fastPinMode(ROW_DECODER_BIT2, OUTPUT);
  fastPinMode(ROW_DECODER_EN1, OUTPUT);
  fastDigitalWrite(ROW_DECODER_EN1, HIGH); // Изначально выключено
  fastPinMode(ROW_DECODER_EN2, OUTPUT);
  fastDigitalWrite(ROW_DECODER_EN2, HIGH); // Изначально выключено
  fastPinMode(ROW_DECODER_EN3, OUTPUT);
  fastDigitalWrite(ROW_DECODER_EN3, HIGH); // Изначально выключено

  // Настройка пинов столбцов
  for (auto pin : colPins) {
    fastPinMode(pin, OUTPUT);
    fastDigitalWrite(pin, LOW);
  }

  // Настройка таймера для динамической индикации
  Timer1.initialize(1000); // 1000Hz (1000 мкс)
  Timer1.attachInterrupt(updateDisplay);

  Serial.begin(9600);
  Serial.println("init complete");
}

void updateDisplayContent()
{
  static uint8_t fill = 0;
  memset(displayBuffer, (fill++ & 0x0F) ? 0xFF : 0, sizeof(displayBuffer));
  fill++;
}

void loop() {
  // Обновляем содержимое буфера
  updateDisplayContent();
  delay(1000);
}
