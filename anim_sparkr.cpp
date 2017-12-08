#include "anim_sparkr.h"

void AnimSparkr::initSeq()
{
    for (int i=0; i<LEDS; i++) {
        seq[i] = i;
    }
}

void AnimSparkr::shuffleSeq() 
{
    for (int i=0; i<LEDS; i++) {
        byte ind = (unsigned int) ( ((byte) rng()) * LEDS / 256);
        if (ind != i) {
            byte tmp = seq[ind];
            seq[ind] = seq[i];
            seq[i] = tmp;
        }
    }
}

void AnimSparkr::setUp() {
    Serial.println("assu");
    AnimGlow::setUp();
    phase = 0;
    curColor = palette->getPalColor((float)rng()/256);
    prevColor = palette->getPalColor((float)rng()/256);
    curColor.println();
    initSeq();
    shuffleSeq();
}

void AnimSparkr::runImpl() {
    AnimGlow::runImpl();

    for (int i=0;i<LEDS;i++) {
        int pos = seq[i];
        leds[pos] = (i > phase)
            ? prevColor 
            : (i == phase) ? sparkleColor : curColor;
        processGlow(i);
    }
    if (phase > LEDS) {
        if (random(SPARK_PROB) == 0) {
            leds[rng() * LEDS / 256] = sparkleColor;
        }    
    }
    phase++;
    if (phase > 3*LEDS) {
        phase = 0;
        prevColor = curColor;
        curColor = palette->getPalColor((float)rng()/256);        
        shuffleSeq();
    }

}