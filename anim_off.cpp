#include "anim.h"
#include "color.h"
#include "palette.h"


void Anim::animOff_SetUp() {
    for (int i=0;i<LEDS;i++) {
        leds[i] = Color(0,0,0);
    }
}

void Anim::animOff_Run() {
}
