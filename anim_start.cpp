#include "anim.h"

void Anim::animStart_SetUp() {
    phase = 0;
}

void Anim::animStart_Run() {
    if (phase < LEDS) {
        leds[phase].r = 255;
        leds[phase].g = 255;
        leds[phase].b = 255;
        for(int i=0; i<LEDS; i++) {
            leds[i].fade(50);
        }        
    } else if (phase >= LEDS) 
    {
        for(int i=0; i<LEDS; i++) {
            int r = LEDS + 255 - phase + (rng() >> 8);
            r = min(r,255); leds[i].r = (byte)max(r,0);
            int g = LEDS + 255 - phase + (rng() >> 8);
            g = min(g,255); leds[i].g = (byte)max(g,0);
            int b = LEDS + 255 - phase + (rng() >> 8);
            b = min(b,255); leds[i].b = (byte)max(b,0);
        }
        phase++;
    }

    phase++;
}
