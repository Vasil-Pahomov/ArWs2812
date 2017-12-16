#include <Arduino.h>

#include "palette.h"

#include "anim.h"


#define ANIMS 6 //number of animations
#define PALS 9 //number of palettes
#define INTERVAL 10000 //change interval, msec

Palette * pals[PALS] = {&PalRgb, &PalRainbow, &PalRainbowStripe, &PalParty, &PalHeat, &PalFire, &PalIceBlue, &PalRachel, &PalBobParis};

Anim anim = Anim();

unsigned long ms = 10000;//startup animation duration, 10000 for "release" AnimStart

int paletteInd = random(PALS);
int animInd = 4;

int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

void setup() {
  Serial.begin(9600);
  Serial.print(F("RAM="));Serial.println(freeRam());
  randomSeed(analogRead(0)*analogRead(1));
  anim.setAnim(animInd);
  anim.setPeriod(20);
  anim.setPalette(pals[0]);
}

void loop() {
  anim.run();
  
  if (millis() > ms) {
    ms = millis() + INTERVAL;
    switch ( (animInd < 0) ? 0 : random(2)) {
      case 0: 
      {
        Serial.print(F("anim->"));
        //int prevAnimInd = animInd;
        //while (prevAnimInd == animInd) animInd = random(ANIMS);
        anim.setAnim(animInd);
        anim.setPeriod(random(5, 50));
        anim.setPalette(pals[paletteInd]);
        break;
      }
      case 1:
      {
        Serial.print(F("pal->"));
        int prevPalInd = paletteInd;
        while (prevPalInd == paletteInd) paletteInd = random(PALS);
        anim.setPalette(pals[paletteInd]);
        Serial.print(paletteInd);
        break;
      }
    }
    Serial.println();
  }
}

