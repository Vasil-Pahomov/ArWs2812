#include <Arduino.h>

#include "anim_test.h"
#include "anim_start.h"
#include "anim_run.h"
#include "anim_pixiedust.h"
#include "palette.h"

#define ANIMS 3 //number of animations
#define PALS 6 //number of palettes
#define INTERVAL 30000 //change interval, msec

Anim * anims[ANIMS];
Palette * pals[PALS] = {&PalRgb, &PalRainbow, &PalRainbowStripe, &PalParty, &PalHeat, &PalFire};

Anim * curAnim;

unsigned long ms = 10000;//startup animation duration
int period = 20;
int paletteInd = random(PALS);

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0)*analogRead(1));

  anims[0] = new AnimStart();
  anims[1] = new AnimRun();
  anims[2] = new AnimPixieDust();

  curAnim = anims[0];
  curAnim->setPeriod(period);
  curAnim->setPalette(pals[0]);
}

void loop() {
  curAnim->run();
  if (millis() > ms) {
    ms = millis() + INTERVAL;
    switch ( (curAnim == anims[0]) ? 0 : random(2)) {
      case 0: 
      {
        int prevAnim = curAnim;
        while (prevAnim == curAnim) curAnim = anims[random(1,ANIMS)];
        period = random(5,50);
        curAnim->setPeriod(period);
        curAnim->setPalette(pals[paletteInd]);
        break;
      }
      case 1:
      {
        int prevPalInd = paletteInd;
        while (prevPalInd == paletteInd) paletteInd = random(PALS);
        curAnim->setPalette(pals[paletteInd]);
        Serial.println("NewPal");
        break;
      }
    }
    

  }
}

