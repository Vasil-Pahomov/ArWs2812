#include "anim.h"

void Anim::glowSetUp()
{
    braPhaseSpd = random(8,13);
    braFreq = random(40,120);
}

void Anim::glowForEachLed(int i)
{
 #if defined(ESP8266)
   int bra = (sint8_t) (braPhase + i * braFreq);
#else
    int bra = (int8_t) (braPhase + i * braFreq);
#endif
    bra = BRA_OFFSET + (abs(bra) >> BRA_AMP_SHIFT);
    leds[i] = leds[i].brightness((int)bra);
}

void Anim::glowRun()
{
    braPhase += braPhaseSpd;
}
