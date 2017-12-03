#ifndef anim_h
#define anim_h
#include <Arduino.h>
#include "ExtNeoPixel.h"
#include "palette.h"

#define PIN 2 // WS2812 pin number
#define LEDS 100 // number of LEDs in the strip 

class Anim {
    
protected:
    // length of animation timeslot (period)
    static byte period;
    static Color *leds;
    static Adafruit_NeoPixel pixels; 
    static Palette *palette;

    // millis for next timeslot 
    unsigned long nextms;
    virtual void runImpl() = 0;

    
public:
    Anim();
    void setPeriod(byte period);
    void setPalette(Palette * pal);
    virtual void setUp();
    void run();
};


#endif