#include "anim_test.h"
#include "color.h"

void AnimTest::runImpl() {
    Color clr(0);
    switch (c)
    {
        case 0:
            clr = Color(255,0,0);
            c++;
            break;
        case 1:
            clr = Color(0,255,0);
            c++;
            break;
        case 2:
            clr = Color(0,0,255);
            c=0;
            break;
        default:
            c = 0;
            break; 
    }
    for (int i=0;i<LEDS;i++) {
        leds[i] = clr; 
    }
}