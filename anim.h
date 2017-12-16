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

//probability of spark when in idle plase
#define SPARK_PROB 3

class Anim {
    
private:
    //Adafruit's class to operate strip
    static Adafruit_NeoPixel pixels; 
    //Color arrays - two for making transition
    static Color leds1[LEDS];
    static Color leds2[LEDS];
    //auxiliary colors array
    static Color ledstmp[LEDS];

    void animStart();
    
    // length of animation timeslot (period)
    byte period;
    // array of Color to work with
    Color *leds;
    Palette *palette;

    // millis for next timeslot 
    unsigned long nextms;
    // millis to transition end
    unsigned long transms;

    int phase;
    int pos;
    int inc;

    //whether to call SetUp on palette change
    //(some animations require full transition with fade, otherwise the colors would change in a step, some not)
    bool setUpOnPalChange;

    Color curColor = Color(0);
    Color prevColor = Color(0);

    Color sparkleColor = Color(0xFFFFFF);

    static byte seq[LEDS];

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
    
    void animSparkr_SetUp();
    void animSparkr_Run();

    void animRandCyc_SetUp();
    void animRandCyc_Run();

    void animStars_SetUp();
    void animStars_Run();

    //void animSpread_SetUp();
    //void animSpread_Run();
public:


    Anim();
    void setPeriod(byte period);
    void setPalette(Palette * pal);
    void setAnim(byte animInd);
    void setUp();
    void run();

};

unsigned int rng();

byte rngb();

#endif