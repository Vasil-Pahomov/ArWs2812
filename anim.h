#ifndef anim_h
#define anim_h
#include <Arduino.h>
#include "ExtNeoPixel.h"
#include "palette.h"

#define PIN 2 // WS2812 pin number
#define LEDS 100 // number of LEDs in the strip 
#define TRANSITION_MS 1000 // transition time between animations, ms

// brigthness animation amplitude shift. true BrA amplitude is calculated as (0..127) value shifted right by this amount
#define BRA_AMP_SHIFT 1
// brigthness animation amplitude offset
#define BRA_OFFSET (222-64)

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

    Color curColor = Color(0);
    Color prevColor = Color(0);

    Color sparkleColor = Color(0xFFFFFF);

    //brigthness animation (BrA) current initial phase
    byte braPhase;
    //braPhase change speed 
    byte braPhaseSpd=5;
    //BrA frequency (spatial)
    byte braFreq=150;

    //glow animation setup
    void glowSetUp();

    //glow animation - must be called for each LED after it's BASIC color is set
    //note this overwrites the LED color, so the glow assumes that color will be stored elsewhere (not in leds[])
    //or computed each time regardless previous leds[] value
    void glowForEachLed(int i);
    
    //glow animation - must be called at the end of each animaton run
    void glowRun();

    //run and setup handlers
    void (Anim::*runImpl)();
    void (Anim::*setUpImpl)();


    //animation implementations
    void animStart_SetUp();
    void animStart_Run();

    void animRun_SetUp();
    void animRun_Run();
    
    void animPixieDust_SetUp();
    void animPixieDust_Run();
    
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