#include "anim_randcyc.h"

/*
void AnimRandCyc::setUp() {
    for (int i=0;i<LEDS;i++) {
        pos[i] = rng();
    }
}

void AnimRandCyc::runImpl() {
    for (int i=0;i<LEDS;i++) {
        Serial.print(pos[i]);Serial.print(",");
        leds[i] = palette->getPalColor((float)pos[i] / 256);
        pos[i]+=rng() >> 6;
    }
    Serial.println();
}
*/