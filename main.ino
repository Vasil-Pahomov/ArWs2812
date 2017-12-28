#include <Arduino.h>

#include "palette.h"

#include "anim.h"


#define ANIMS 7 //number of animations
#define PALS 7 //number of palettes
#define INTERVAL 30000 //change interval, msec

Palette * pals[PALS] = {&PalRgb, &PalRainbow, &PalParty, &PalRainbowStripe, &PalHeat, &PalFire, &PalIceBlue};

Anim anim = Anim();

unsigned long ms = 10000;//startup animation duration, 10000 for "release" AnimStart

int paletteInd = -1;
int animInd = -1;

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
  anim.doSetUp();
}

void loop() {
  anim.run();
  
  if (millis() > ms) {
    ms = millis() + INTERVAL;
    animInd++;
    if (animInd >= ANIMS) animInd = 0;
    paletteInd++;
    if (paletteInd >= PALS) paletteInd = 0;
    anim.setAnim(animInd);
    anim.setPalette(pals[paletteInd]);
    anim.doSetUp();
  }
}

