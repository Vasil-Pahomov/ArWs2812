#include <Arduino.h>
#include "ExtNeoPixel.h"
#include "color.h"
#include "palette.h"
#include "anim.h"
#include "brightness.h"

Anim::Anim() 
{
    leds1 = (Color *)malloc(3*LEDS);
    memset(leds1, 0, 3*LEDS);
    leds2 = (Color *)malloc(3*LEDS);
    memset(leds2, 0, 3*LEDS);        

    pixels.begin();
    pixels.show(); // turn of all LEDs

    nextms = millis();
}

void Anim::setPeriod(byte period) {
    this->period = period;
}

void Anim::setPalette(Palette * pal) {
    this->palette = pal;
    setUp();
}

void Anim::run()
{    
    if ( millis()<=nextms) {
        digitalWrite(LED_BUILTIN, HIGH);
        return;
    }
    digitalWrite(LED_BUILTIN, LOW);
    nextms=millis() + period;
    
    if (runImpl != NULL)
    {
        (this->*runImpl)();
    }

    //transition coef, if within 0..1 - transition is active
    //changes from 1 to 0 during transition, so we interpolate from current color to previous
    float transc = (float)((long)transms - (long)millis()) / TRANSITION_MS;
    Color * leds_prev = (leds == leds1) ? leds2 : leds1;
    
    if (transc > 0) {
        for(int i=0; i<LEDS; i++) {
            //transition is in progress
            Color c = leds[i].interpolate(leds_prev[i], transc);
            pixels.setPixelColor(i, pixels.Color(BRI[c.r], BRI[c.g], BRI[c.b]));
        }
    } else {
        for(int i=0; i<LEDS; i++) {
            //regular operation
            pixels.setPixelColor(i, pixels.Color(BRI[leds[i].r], BRI[leds[i].g], BRI[leds[i].b]));
        }
    }
  
    pixels.show();
    digitalWrite(LED_BUILTIN, HIGH);
    
}

void Anim::setUp()
{
    pinMode(LED_BUILTIN, OUTPUT);  
    transms = millis() + TRANSITION_MS;

    //switch operation buffers (for transition to operate)
    
    if (leds == leds1) {
        leds = leds2;
    } else {
        leds = leds1;
    }

    if (setUpImpl != NULL) {
        (this->*setUpImpl)();
    }
}

void Anim::setAnim(byte animInd)
{
    switch (animInd) {
        case 0: 
            setUpImpl = &Anim::animRun_SetUp;
            runImpl = &Anim::animRun_Run;
        break;
        case 1: 
            setUpImpl = &Anim::animPixieDust_SetUp;
            runImpl = &Anim::animPixieDust_Run;
        break;        
        case 2: 
            setUpImpl = &Anim::animSparkr_SetUp;
            runImpl = &Anim::animSparkr_Run;
        break;        
        case 3: 
            setUpImpl = &Anim::animRandCyc_SetUp;
            runImpl = &Anim::animRandCyc_Run;
        break;        
        default:
            setUpImpl = &Anim::animStart_SetUp;
            runImpl = &Anim::animStart_Run;
        break;
    }
}



unsigned int rng() {
    static unsigned int y = 0;
    y += micros(); // seeded with changing number
    y ^= y << 2; y ^= y >> 7; y ^= y << 7;
    return (y);
}

byte rngb() {
    return (byte)rng();
}


Adafruit_NeoPixel Anim::pixels = Adafruit_NeoPixel(LEDS, PIN, NEO_GRB + NEO_KHZ800); 
Color * Anim::leds1 = 0;
Color * Anim::leds2 = 0;
