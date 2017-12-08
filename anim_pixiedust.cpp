#include "anim_pixiedust.h"
#include "color.h"
#include "palette.h"

void AnimPixieDust::setUp() {
    Anim::setUp();
    phase = 0;
    curColor = palette->getPalColor((float)rng()/256);
    curColor.println();
    prevColor = palette->getPalColor((float)rng()/256);
    prevColor.println();
    braPhaseSpd = random(8,13);
    braFreq = random(40,120);
}

void AnimPixieDust::runImpl() {
    byte braPh = braPhase;
    for (int i=0;i<LEDS;i++) {
        leds[i] = (i > phase) ? prevColor : curColor;
        int bra = (char)braPh;
        bra = BRA_OFFSET + (abs(bra) >> BRA_AMP_SHIFT);
        leds[i] = leds[i].brightness((int)bra);
       
        braPh += braFreq;
    }
    braPhase += braPhaseSpd;

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