#include "anim_run.h"
#include "color.h"
#include "palette.h"

void AnimRun::setUp() {
    Anim::setUp();
    pos = 0;
    inc = (random(100)+10)/5000.0;
    if (random(10) < 5) {
        inc = -inc;
    }
}

void AnimRun::runImpl() {
    
    float p = pos;
    for (int i=0;i<LEDS;i++) {
        Color c = palette->getPalColor(p);
        leds[i] = c;
   
        p = p + inc;
        if (p > 1) {
            p = p - 1;
        } else if (p < 0) {
            p = p + 1;
        }
    }
    pos = pos + 0.005;
}