#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>


#include "palette.h"

#include "anim.h"


#define ANIMS 7 //number of animations
#define PALS 7 //number of palettes
#define INTERVAL 5000 //change interval, msec

Palette * pals[PALS] = {&PalRgb, &PalRainbow, &PalRainbowStripe, &PalParty, &PalHeat, &PalFire, &PalIceBlue};

Anim anim = Anim();

//11 and 12 are RX (from BT) and TX (to BT) pin numbers
SoftwareSerial bt(11,12);


unsigned long ms = 10000;//startup animation duration, 10000 for "release" AnimStart

int paletteInd = random(PALS);
int animInd = -1;

int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

extern Adafruit_NeoPixel pixels;

void setup() {
  pixels.begin();
  bt.begin(9600);
  bt.print(F("RAM="));bt.println(freeRam());
  randomSeed(analogRead(0)*analogRead(1));
  anim.setAnim(animInd);
  anim.setPeriod(20);
  anim.setPalette(pals[0]);
  anim.doSetUp();
  bt.listen();
  
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
    switch ( (animInd < 0) ? 0 : random(2)) {
      case 0: 
      {
        int prevAnimInd = animInd;
        while (prevAnimInd == animInd) animInd = random(ANIMS);
        anim.setAnim(animInd);
        anim.setPeriod(random(20, 40));
        anim.setPalette(pals[paletteInd]);
        anim.doSetUp();
        break;
      }
      case 1:
      {
        int prevPalInd = paletteInd;
        while (prevPalInd == paletteInd) paletteInd = random(PALS);
        anim.setPalette(pals[paletteInd]);
        break;
      }
    }
    bt.print(F(">"));bt.print(animInd);bt.print(F("\t"));bt.println(paletteInd);
    bt.listen();
  }
  
  if (bt.available() >= 2) {
    //todo: range checks!
    anim.setAnim(bt.read()-48);
    anim.setPalette(pals[bt.read()-48]);
    anim.doSetUp();
    ms = millis() + INTERVAL;
    //while (bt.available()) bt.read();
    bt.print(F("="));
  }
  /**/
}

