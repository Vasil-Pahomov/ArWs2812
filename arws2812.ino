#include <Adafruit_NeoPixel.h>


#include "palette.h"

#include "anim.h"


#define ANIMS 7 //number of animations
#define PALS 7 //number of palettes
#define INTERVAL 30000 //change interval, msec

#define USE_START_ANIMATION //start animation is used in cycling as well as other animations

Palette * pals[PALS] = {&PalRgb, &PalRainbow, &PalRainbowStripe, &PalParty, &PalHeat, &PalFire, &PalIceBlue};

Anim anim = Anim();

unsigned long ms = 10000;//startup animation duration, 10000 for "release" AnimStart

int paletteInd = random(PALS);
int animInd = -1;

extern Adafruit_NeoPixel pixels;

void setup() {
  pixels.begin();
  Serial.begin(9600);
  Serial.println(F("Start"));
  randomSeed(analogRead(0)*analogRead(1));
  anim.setAnim(animInd);
  anim.setPeriod(20);
  anim.setPalette(pals[0]);
  anim.doSetUp();
}

void loop() {
  /* this piece of code checks for looping while trying to find different colors
  for (int pi=0;pi<PALS;pi++) {
    int c = 0;
    
    Serial.print(F("pi="));Serial.print(pi);
    Color c1 = pals[pi]->getPalColor((float)rngb()/256);
    Color c2 = c1;
    while (c1.isCloseTo(c2)) {
      c = c + 1;
      c2 = pals[pi]->getPalColor((float)rngb()/256);
    }
    Serial.print(F(" c="));Serial.println(c);
  }
  /**/
  
  anim.run();
  
  if (millis() > ms) {
    ms = millis() + INTERVAL;
    switch ( (animInd < 0) ? 0 : random(1)) {
      case 0: 
      {
        Serial.print(F("anim->"));
        int prevAnimInd = animInd;
#ifdef USE_START_ANIMATION
        while (prevAnimInd == animInd) animInd = random(ANIMS+1) - 1;
        if (animInd < 0) ms = millis() + 10000;//startup animation has fixed 10 seconds length
#else
        while (prevAnimInd == animInd) animInd = random(ANIMS);
#endif        
        anim.setAnim(animInd);
        anim.setPeriod(random(20, 40));
        anim.setPalette(pals[paletteInd]);
        anim.doSetUp();
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
  /**/
}
