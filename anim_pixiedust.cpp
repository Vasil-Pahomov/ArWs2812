#include "anim.h"
#include "color.h"
#include "palette.h"

#define DUST_LENGTH 20
void Anim::animPixieDust_SetUp() {
    Serial.println("PixieDust");
    phase = 0;
    curColor = palette->getPalColor((float)rng()/256);
    prevColor = palette->getPalColor((float)rng()/256);
    glowSetUp();
}

void Anim::animPixieDust_Run() {

    for (int i=0;i<LEDS;i++) {
        leds[i] = (i > phase) ? prevColor : curColor;
        glowForEachLed(i);
    }
    glowRun();
    
    for (int k = phase-DUST_LENGTH/2; k < (phase + DUST_LENGTH/2); k++ ) {
        if (k >= 0 && k < LEDS) {
            int mix = abs(k-phase) * 255 / DUST_LENGTH + random(-100, 100);
            if (mix < 0) { 
                mix = 0;
            } else if (mix > 255) {
                mix = 255;
            }
            leds[k] = sparkleColor.interpolate(leds[k], (float)mix/255);
        }
    }
    phase++;
    if (phase >= 4*LEDS) {
        phase = 0;
        prevColor = curColor;
        curColor = palette->getPalColor((float)rng()/256);        
    }
}