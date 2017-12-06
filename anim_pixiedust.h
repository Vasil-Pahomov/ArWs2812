#ifndef anim_pixiedust_h
#define anim_pixiedust_h

#include "anim.h"

// brigthness animation amplitude shift. true BrA amplitude is calculated as (0..127) value shifted right by this amount
#define BRA_AMP_SHIFT 2
// brigthness animation amplitude offset
#define BRA_OFFSET 255-32
// length of "dust" transition between colors, number of LEDs
#define DUST_LENGTH 10

class AnimPixieDust : public Anim {
private:
    //index of current color in palette
    byte palInd;
    //index of previous color in palette
    byte palIndPrev;
    //brigthness animation (BrA) current initial phase
    byte braPhase;
    //braPhase change speed 
    byte braPhaseSpd=5;
    //BrA frequency (spatial)
    byte braFreq=150;

    //phase of the animation (in fact - index of LED that changes color)
    byte phase = 0; 

protected:
        virtual void runImpl();
        virtual void setUp();
};
#endif
