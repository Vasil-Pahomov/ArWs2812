#include "anim.h"
#include "color.h"
#include "palette.h"

//seq keeps phases: 0..127 increasing, 128..255 decreasing, ends at 255 (steady off)
//ledstmp keeps color of stars

void Anim::animStars_SetUp() {
    Serial.print(F("(stars)"));
    //inc is (average) interval between appearance of new stars
    inc = random (5, 10);

    //reset all phases
    memset(seq, 255, LEDS); 
}

void Anim::animStars_Run() {   
    for (byte i=0;i<LEDS;i++) {
        byte phi = seq[i];
        if (phi != 255) {
            Color col = ledstmp[i];
            if (phi <= 127) {
                leds[i] = col.brightness(phi << 1);
            } else {
                leds[i] = col.brightness((255-phi) << 1);
            }
            //leds[i].println();
            seq[i]++;
        } else {
            leds[i].r = 0;
            leds[i].g = 0;
            leds[i].b = 0;
        }
    }

    if (random(inc) == 0) {
        byte pos = random(LEDS);
        if (seq[pos] == 255) {
            seq[pos] = 0;
            ledstmp[pos] = palette->getPalColor((float)rngb()/256);
        }
    }
}