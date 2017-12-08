#ifndef anim_h
#define anim_h
#include <Arduino.h>
#include "ExtNeoPixel.h"
#include "palette.h"

#define PIN 2 // WS2812 pin number
#define LEDS 100 // number of LEDs in the strip 
#define TRANSITION_MS 1000 // transition time between animations, ms

class Anim {
    
private:
    //Adafruit's class to operate strip
    static Adafruit_NeoPixel pixels; 
    //Color arrays - two for making transition
    static Color *leds1;
    static Color *leds2;

    void animStart();
    
protected:
    // length of animation timeslot (period)
    static byte period;
    // array of Color to work with
    static Color *leds;
    static Palette *palette;



    // millis for next timeslot 
    unsigned long nextms;
    // millis to transition end
    unsigned long transms;

    int phase;
    int pos;
    int inc;

    void (Anim::*runImpl)();
    void (Anim::*setUpImpl)();


    //animation implementations
    void animStart_SetUp();
    void animStart_Run();

    void animRun_SetUp();
    void animRun_Run();
    
    
public:


    Anim();
    void setPeriod(byte period);
    void setPalette(Palette * pal);
    void setAnim(byte animInd);
    void setUp();
    void run();

};

unsigned int rng();

#endif