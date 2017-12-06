#include "anim_pixiedust.h"
#include "color.h"
#include "palette.h"

void AnimPixieDust::setUp() {
    Anim::setUp();
    phase = 0;
    palInd = 1;
    palIndPrev = 0;
    braPhaseSpd = random(8,13);
    braFreq = random(40,299);
}

void AnimPixieDust::runImpl() {
    byte braPh = braPhase;
    //int bra1 = (char)braPh;
    //bra1 = BRA_OFFSET + (abs(bra1) >> BRA_AMP_SHIFT);
    //Serial.print("ph=");Serial.print(braPhase); Serial.print(" b=");Serial.println(bra1);
    for (int i=0;i<LEDS;i++) {
        leds[i] = (i > phase) ? palette->colors[palIndPrev] : palette->colors[palInd];
        int bra = (char)braPh;
        bra = BRA_OFFSET + abs(bra) >> BRA_AMP_SHIFT;
        //Serial.print(" i=");Serial.print(i);Serial.print(" b=");Serial.print((int)bra);
        //Serial.print(" r=");Serial.print(leds[i].r);Serial.print(" g=");Serial.print(leds[i].g);Serial.print(" b=");Serial.print(leds[i].b);
        leds[i] = leds[i].brightness((int)bra);
        //Serial.print(" r=");Serial.print(leds[i].r);Serial.print(" g=");Serial.print(leds[i].g);Serial.print(" b=");Serial.println(leds[i].b);
        
        braPh += braFreq;
    }
    braPhase += braPhaseSpd;

    /*for (int i=phase-DUST_LENGTH/2;i++;i<phase+DUST_LENGTH/2) {
        float mix = (float)abs(i-phase)/DUST_LENGTH;
        byte dustBr = random(100,255);
        leds[i] = Color(dustBr, dustBr, dustBr).interpolate(leds[i], mix);
        Serial.print("i=");Serial.println(i);
    }*/
    phase++;
    if (phase == 255) {
        phase = 0;
        palIndPrev = palInd;
        palInd++;
        if (palInd > palette->numColors) {
            palInd = 0;
        }
    }

}