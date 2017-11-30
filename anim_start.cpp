#include <Arduino.h>
#include "anim.h"

class AnimStart : public Anim 
{
private:
    int phase_c = 0;
    int phase=0;
protected:
        virtual void runImpl();
};

void AnimStart::runImpl() {

    if (phase_c == 0) {
        if (phase < LEDS) {
        leds[phase].r = 255;
        leds[phase].g = 255;
        leds[phase].b = 255;
        } else if (phase == LEDS) {
        for(int i=0; i<LEDS; i++) {
            leds[i].r = 255;
            leds[i].g = 255;
            leds[i].b = 255;
        }
        }
    }

    if (phase <= LEDS) {
        for(int i=0; i<LEDS; i++) {
        leds[i].fade(5);
        }
    } else {
        for(int i=0; i<LEDS; i++) {
        leds[i].fade3(random(0,3), random(0,3), random(0,3));
        }
    }

    if ( phase_c++ > 5) {
      phase++;
      phase_c = 0;
    }
}
