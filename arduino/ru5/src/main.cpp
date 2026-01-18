#include <Arduino.h>
#include <GyverIO.h>
#include <STM32TimerInterrupt.h>

// Частота тактирования, Гц
const uint32_t CLK = 20000;
// Интервал изменения кадра, мс
const uint32_t FRAME_TICK = 10000;
// Интервал опроса клавиатуры, мс
const uint32_t KEY_TICK = 25;

class pinIn {
  protected:
    const uint32_t pin;
  public:
    pinIn(const uint32_t _pin): pin(_pin) {}
    void init() {
      pinMode(pin, INPUT_PULLUP);
    }
    bool read() {
      return (digitalRead(pin));
    }
};

class pinOut : public pinIn {
  public:
    using pinIn::pinIn;
    using pinIn::read;
   void init(bool _state = false) {
    write(_state);
    pinMode(pin, OUTPUT);
   }
    void write(bool _state) {
      digitalWrite(pin, _state);
    }
    void set() {
      write(true);
    }
    void reset() {
      write(false);
    }
    bool toggle() {
      auto result = !read();
      write(result);
      return result;
    }
};

pinOut ledPin = {PC13};
pinIn  keyPin = {PA0 };

enum pins {
  Clk,
  Start,
  WE,
  DI,
  cnt
};

pinOut ctlPins[pins::cnt] = {{PB9}, {PB8}, {PB7}, {PB6}};
pinIn  pinFRM = {PB5};

// Буфер дисплея (10x24)
uint16_t displayBuffer[24] = {0};
const uint16_t displayBufferCount = sizeof(displayBuffer) / sizeof(displayBuffer[0]);

STM32Timer ITimerClk(TIM1);
STM32Timer ITimerLed(TIM2);

bool traceMode = false;
bool update = false;

// Загрузка образа в ОЗУ
void loadRam(const uint16_t *_img, const bool _raise = false) {
  static enum steps {
    free,
    waitRaise,
    waitFall,
    loadData,
  } step = free;
  static const uint16_t *img = nullptr;
  static auto cnt = 0;

  switch (step) {
    case steps::waitRaise:
      if (!_raise || !pinFRM.read()) break;
      step = steps::waitFall;
      break;
    case steps::waitFall:
      if (!_raise || pinFRM.read()) break;
      step = steps::loadData;
      // Первые два шага нужны для детектирования перехода линии A8 в 0
      // Это означает, что произошел сброс счетчика строк и сейчас можно начинать писать кадр
    case steps::loadData:
      if (!_raise) break;
      ctlPins[pins::DI].write(img[cnt/10] & (1 << cnt%10));
      /// TODO здесь надо дернуть WE но с задержкой в 1 мкс примерно на 1 мкс
      ctlPins[pins::WE].write(1);
      ctlPins[pins::WE].write(1);
      ctlPins[pins::WE].write(0);
      ctlPins[pins::WE].write(0);
      ctlPins[pins::WE].write(1);
      if (++cnt >= displayBufferCount * 10) step = steps::free;
      break;
    default:
      if (_img == nullptr) break;
      img = _img;
      cnt = 0;
      step = steps::waitRaise;
  }
}

// Прерывание для обновления дисплея
void updateClk() {
  loadRam(nullptr, ctlPins[pins::Clk].toggle());
}

// Прерывание для обработки светодиода и кнопки
void updateLed() {
  static uint16_t key = 0;
  static uint8_t led = 0;

  if (!keyPin.read())
  {
    if (key < 255) key++;
    if (key == 50)
    {
      // long push
      if (traceMode){
        ITimerClk.enableTimer();

      }
      else{
        ITimerClk.disableTimer();
      }
      traceMode = !traceMode;
      ledPin.write(!traceMode);
    }
  }
  else
  {
    if (key > 2 && key < 50)
    {
      // push up
      if (traceMode)
      {
        updateClk();
      }
      else
      {
        update = true;
        ledPin.toggle();
      }
    }
    key = 0;
  }
}

void setup() {
  // Настройка пинов
  for (auto &pin : ctlPins) {
    pin.init(true);
  }
  pinFRM.init();

  keyPin.init();
  ledPin.init(false);

  // Настройка таймеров
  // Делим частоту на 2 т.к. программно формируем и фронт и спад
  ITimerClk.attachInterruptInterval(1000000UL / 2 / CLK, updateClk);
  ITimerLed.attachInterruptInterval(KEY_TICK * 1000, updateLed);
}

void updateDisplayContent()
{
  static auto step = 2;
  switch (step){
    case 0:
    case 1:
      static uint16_t fill = 0x5555;
      for(int i = displayBufferCount; i >= 0; displayBuffer[i--] = (i & 1) ? fill : ~fill);
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
  if (update) {
    updateDisplayContent();
    ITimerLed.disableTimer();
    ITimerClk.disableTimer();
    loadRam(displayBuffer);
    ITimerClk.enableTimer();
    ITimerLed.enableTimer();
    update = false;
  }
}
