#include "anim.h"
#include "color.h"
#include "palette.h"


void Anim::animRun_SetUp() {
    Serial.println("Run");
    pos = 0;
    inc = 1 + (((byte)rng()) >> 5);
    if (random(10) > 5) {
        inc = -inc;
    }
}

void Anim::animRun_Run() {
    
    int p = pos;
    for (int i=0;i<LEDS;i++) {
        Color c = palette->getPalColor((float)p/256.0);
        leds[i] = c;
   
        p = p + inc;
        if (p >= 256) {
            p = p - 256;
        } else if (p < 0) {
            p = p + 256;
        }
    }
    pos = pos + 1;
}