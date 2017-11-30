#ifndef anim_h
#define anim_h
#include <Arduino.h>
#include "ExtNeoPixel.h"
#include "palette.h"

#define PIN 2 // номер порта к которому подключен модуль
#define LEDS 100 // количество светодиодов 

class Anim {
    
protected:
    //период "такта" анимации, мс
    byte period;
    //значение millis() для следующего такта
    unsigned long nextms;

    Palette *palette;

    static Color *leds;
    static Adafruit_NeoPixel pixels; 

    virtual void runImpl() = 0;
    
public:
    Anim();
    void setPeriod(byte period);
    void setPalette(Palette * pal);
    void run();
};


#endif