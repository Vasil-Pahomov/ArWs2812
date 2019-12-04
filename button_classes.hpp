// ver 3.0. 2019-12-XX.
// constructor [ heX ]   https://github.com/heX16
#ifndef BUTTON_CLASSES_HEX_LIB
#define BUTTON_CLASSES_HEX_LIB

#include "small_timer.hpp"

//NOTE: Все описанные ниже классы кнопок очень похожи друг на друга.
//      Я пробовал создать один базовый класс и придумать модель "ветвления" вариаций.
//      Но любой опробованный метод или расходует оперативную память, или не оптимально компилируется, или превращает исходник в хаос и черную магию.
//      Поэтому просто копирование - это тупо и очень просто.

//todo: csButtonLongClicksFaster сделать копию класса с поддержкой ускорения кликов. это когда через пару секунд повторы кликов идут еще быстрее.

/* Example:
csButtonLongPress btn1;
void loop() {
  btn1.run(digitalRead(2));
  if (btn.flags.click)
    Serial.println(F("Click"))
  else if (btn.flags.longPress)
    Serial.println(F("longPress"));
}
*/

// класс описывающий кнопку с поддержкой долгого нажатия
class csButtonLongPress {
public:
  csTimerShort tBtn; // антидребезг
  csTimerShort tLongPress; // таймер длительности длинного клика

  // структура для хранения флагов (каждый флаг занимает один бит)
  struct tFlags {
    unsigned click : 1;     // обычный клик обнаружен (однакратная выдача! - флаг удерживается только один скан)
    unsigned longPress : 1;  // длинное нажатие обнаружено (однакратная выдача! - флаг удерживается только один скан)
    unsigned _pushed : 1;  // private! флаг-защелка нажатия на кнопку. по совместительству просто флаг того что кнопка вроде нажата (без выдержки времени!).
    unsigned _pushedShort : 1;  // private! флаг-защелка
    unsigned _pushedLong : 1;  // private! флаг-защелка
  } flags;

  csButtonLongPress() {
    flags.click = false;
    flags.longPress = false;
    flags._pushed = false;
    flags._pushedShort = false;
    flags._pushedLong = false;
  }

  // основной алгоритм.
  // @button - текущее состояние DI кнопки. True - нажата. False - отжата.
  // @timeClickDebounce - время минимального времени нажатия на кнопку.
  // @timeLongPress - время нажатия на кнопку которое будет считаться долгим нажатием.
  // P.S. алгоритм обработки кнопок получился не тривиальным...
  void run(const bool button, const uint16_t timeLongPress, const uint16_t timeClickDebounce = 50) {
    flags.click = false;
    flags.longPress = false;

    // обработка одинарного нажатия
    if (flags._pushedShort && ! button) {
      //LOG("button click");
      flags.click = true;
    }
    // обработка длинного нажатия
    if (flags._pushedLong && ! button) {
      //LOG("button Long Clk");
      flags.longPress = true;
    }

    // обработка физики (антидребезг и прочее)
    if (button) {
      if (flags._pushed == false) {
        //LOG("button pushed");
        // запускаем оба таймера - после отпускания кнопки будем смотреть какой сработал
        tBtn.start(timeClickDebounce);
        tLongPress.start(timeLongPress);
        // взводим защелку чтобы избежать повторного запуска таймеров
        flags._pushed = true;
      }
    } else {
      // кнопку отпустили
      flags._pushed = false;
      flags._pushedShort = false;
      flags._pushedLong = false;
      tBtn.stop();
      tLongPress.stop();
    }

    // Кнопка еще нажата и отработал таймер - взводим короткого клика. Теперь если кнопку отпустят то может сработать "click".
    if(tBtn.run()) {
      flags._pushedShort = true;
    }

    // Кнопка еще нажата и прошло много времени - выдаем длинный клик. Флаг короткого клика сбрасываем. Теперь если кнопку отпустят то сработает "longPress".
    if(tLongPress.run()) {
      flags._pushedLong = true;
      flags._pushedShort = false;
    }
  }
};

template <const uint16_t tpTimeLongPress, const uint16_t tpTimeClickDebounce = 50, const bool inverse = false>
class csButtonLongPressDef : public csButtonLongPress {
public:
  using csButtonLongPress::run;

  inline void run(bool button) {
    if (inverse)
      csButtonLongPress::run( ! button, tpTimeLongPress, tpTimeClickDebounce);
    else
      csButtonLongPress::run(button, tpTimeLongPress, tpTimeClickDebounce);
  }
};

////////////////////////////////////

// класс с поддержкой долгого нажатия с выдачей поторных кликов. интервал между повторами и стартом повторов одинаковый.
class csButtonLongClicks {
public:
  csTimerShort tBtn; // антидребезг
  csTimerShort tLongPress; // таймер длительности длинного клика

  // структура для хранения флагов (каждый флаг занимает один бит)
  struct tFlags {
    unsigned click : 1;     // обычный клик обнаружен (однакратная выдача! - флаг удерживается только один скан)
    unsigned clickRepeat : 1;  // доп флаг - чтобы можно было определить что нажатие идет в режиме повтора при удержании кнопки (однакратная выдача!)
    unsigned _pushed : 1;  // private! флаг-защелка нажатия на кнопку. по совместительству просто флаг того что кнопка вроде нажата (без выдержки времени!).
    unsigned _pushedShort : 1;  // private! флаг-защелка
  } flags;

  csButtonLongClicks() {
    flags.click = false;
    flags.clickRepeat = false;
    flags._pushed = false;
    flags._pushedShort = false;
  }

  // основной алгоритм.
  // @button - текущее состояние DI кнопки. True - нажата. False - отжата.
  // @timeClickDebounce - время минимального времени нажатия на кнопку.
  // @timeLongPress - время нажатия на кнопку которое будет считаться долгим нажатием.
  // P.S. алгоритм обработки кнопок получился не тривиальным...
  void run(const bool button, const uint16_t timeLongPress, const uint16_t timeClickDebounce = 50) {
    flags.click = false;
    flags.clickRepeat = false;

    // обработка одинарного нажатия
    if (flags._pushedShort && ! button) {
      //LOG("button click");
      flags.click = true;
    }

    // обработка физики (антидребезг и прочее)
    if (button) {
      if (flags._pushed == false) {
        //LOG("button pushed");
        // запускаем оба таймера - после отпускания кнопки будем смотреть какой сработал
        tBtn.start(timeClickDebounce);
        tLongPress.start(timeLongPress);
        // взводим защелку чтобы избежать повторного запуска таймеров
        flags._pushed = true;
      }
    } else {
      // кнопку отпустили
      flags._pushed = false;
      flags._pushedShort = false;
      tBtn.stop();
      tLongPress.stop();
    }

    // Кнопка еще нажата и отработал таймер - взводим короткого клика. Теперь если кнопку отпустят то может сработать "click".
    if(tBtn.run()) {
      flags._pushedShort = true;
    }

    // Кнопка еще нажата и прошло много времени - выдаем клик.
    if(tLongPress.run()) {
      //LOG("button Long Clk");
      tLongPress.start(timeLongPress);
      // выдаем клик
      flags.click = true;
      // помечаем что этот клик от длинного нажатия
      flags.clickRepeat = true;
      // сбрасываем флаг-защелку - чтобы при отпускании кнопки не произошло лишнего нажатия
      flags._pushedShort = false;
    }
  }
};

template <const uint16_t tpTimeLongPress, const uint16_t tpTimeClickDebounce = 50, const bool inverse = false>
class csButtonLongClicksDef : public csButtonLongClicks {
public:
  using csButtonLongClicks::run;

  inline void run(bool button) {
    if (inverse)
      csButtonLongClicks::run( ! button, tpTimeLongPress, tpTimeClickDebounce);
    else
      csButtonLongClicks::run(button, tpTimeLongPress, tpTimeClickDebounce);
  }
};

#endif // BUTTON_CLASSES_HEX_LIB
