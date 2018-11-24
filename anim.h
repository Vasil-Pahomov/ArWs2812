#ifndef anim_h
#define anim_h
#include <Adafruit_NeoPixel.h>
#include "palette.h"


// brigthness animation amplitude shift. true BrA amplitude is calculated as (0..127) value shifted right by this amount
#define BRA_AMP_SHIFT 1
// brigthness animation amplitude offset
#define BRA_OFFSET (222-64)

//probability of spark when in idle plase
#define SPARK_PROB 3

class Anim {
    
private:
    //Led color arrays - two for making transition
    //each element transforms to color with the corresponding palette
    static byte leds1[LEDS];
    static byte leds2[LEDS];
    static byte bri1[LEDS];
    static byte bri2[LEDS];
    //auxiliary array
    static byte ledstmp[LEDS];

    void animStart();
    
    // length of animation timeslot (period)
    byte period;
    // arrays of colors and brightness to work with
    byte *leds, *bri;
    Palette *palette;
    Palette *prev_palette;

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

    byte curColor = 0;
    byte prevColor = 0;

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

    void setUp();

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

    void animSpread_SetUp();
    void animSpread_Run();

    void animFly_SetUp();
    void animFly_Run();

    void animBT_SetUp();
    void animBT_Run();

public:


    Anim();
    void setPeriod(byte period);
    void setPalette(Palette * pal);
    void setAnim(byte animInd);
    bool run();//returns true if actual change has completed, or false if it's dummy call (previous call was too recent in time)
    void doSetUp();

};

unsigned int rng();

byte rngb();

#endif
