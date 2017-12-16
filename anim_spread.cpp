#include "anim.h"
#include "color.h"
#include "palette.h"

/*
void Anim::animSpread_SetUp() {
    Serial.print(F("spread"));
    inc = random(5,15);
}

void Anim::animSpread_Run() {
    for (int i=1;i<LEDS;i++) {
        leds[i] = leds[i].interpolate(leds[i-1], 0.5);
    }

    if (random(inc) == 0) {
        leds[random(LEDS)] = palette->getPalColor((float)rngb()/256);
    }        
}
*/