// use button
#define BUTTON_NEXT_EFF
// use Bluetooth
#define BT
//whether to use hardware serial to communicate Bluetooth. Software serial is used otherwise
//#define BTHS
//if defined, debug data is output to hardware serial port. REMEMBER TO REMOVE this definition once BTHS is set
//#define DEBUG

#include <Adafruit_NeoPixel.h>

#ifdef BT
#define _SS_MAX_RX_BUFF 8 //lower SoftwareSerial's receive buffer to conserve some RAM
#include <SoftwareSerial.h>
#endif

#include "palette.h"
#include "anim.h"
#include "commands.h"
#include "small_timer.hpp"
#ifdef BUTTON_NEXT_EFF
#include "button_classes.hpp"
#endif

#define ANIMS 7 //number of animations
#define PALS 7 //number of palettes
#define INTERVAL 10000 //change interval, msec


constexpr auto pinButtonNextEff = A0; // button pin for next effect
constexpr bool buttonNextEffInverse = true; // options: inverse "button for next effect"

Palette * pals[PALS] = {&PalRgb, &PalRainbow, &PalRainbowStripe, &PalParty, &PalHeat, &PalFire, &PalIceBlue};

Anim anim = Anim();

#ifdef BT
#ifndef BTHS
//11 and 12 are RX (from BT) and TX (to BT) pin numbers
SoftwareSerial bt(11,12);
#else
#define bt Serial
#endif
#endif

byte command[COMMAND_LENGTH-1];   //BT command buffer
bool commandComplete;  //flag indicating whether the command is complete or not
byte cmdPos;  //position inside command buffer; 0 means buffer is empty; 1 means command marker received; 2...5 means command data received. Value of 5 means the command is fully received

#ifdef BUTTON_NEXT_EFF
csButtonLongClicksDef<1000, 50, buttonNextEffInverse> btnNextEff;
bool disableAutoChangeEffects = false;
csTimerDef <1000> DetectTripleFastClickTimer;
uint8_t DetectTripleFastClickCounter = 0;
#else
constexpr bool disableAutoChangeEffects = false;
#endif // BUTTON_NEXT_EFF

csTimerDef <INTERVAL> EffectAutoChangeTimer; // auto change animation effect, interval timer

int paletteInd;
int animInd = -1;

int freeRam () {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

extern Adafruit_NeoPixel pixels;

void setup() {
#ifdef DEBUG
  Serial.begin(9600);
  Serial.print(F("RAM="));Serial.println(freeRam());
#endif

  pixels.begin();
#ifdef BT
  bt.begin(9600);
#endif
  randomSeed(analogRead(0)*analogRead(1));
  paletteInd = random(PALS);
  anim.setAnim(animInd);
  anim.setPeriod(20);
  anim.setPalette(pals[0]);
  anim.doSetUp();
#ifdef BT
#ifndef BTHS
  bt.listen();
#endif
#endif

#ifdef BUTTON_NEXT_EFF
  pinMode(pinButtonNextEff, INPUT_PULLUP);
#endif // BUTTON_NEXT_EFF

  EffectAutoChangeTimer.start(10000); // 10000 for "release" AnimStart
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

  bool needChange = EffectAutoChangeTimer.run();

#ifdef BUTTON_NEXT_EFF
  btnNextEff.run(digitalRead(pinButtonNextEff));
#endif // BUTTON_NEXT_EFF


#ifdef BT
  if (bt.available()) {
    if (cmdPos == 0) { //wait for command marker when command buffer is empty, discard everything that doesn't match command marker
      byte b;
      do {
        b = bt.read();
#ifdef DEBUG
        Serial.print(b);Serial.write(32);
#endif
      } while (bt.available() && b != COMMAND_MARKER);
      if (b == COMMAND_MARKER) cmdPos = 1;
    }
    while (bt.available() && cmdPos < COMMAND_LENGTH) {
       byte b = bt.read();
#ifdef DEBUG
       Serial.print(b);Serial.write(32);
#endif
       command[(cmdPos++)-1] = b;
    }
    if (cmdPos >= COMMAND_LENGTH) {
      byte cs = 0;
      for (byte i=0;i<(COMMAND_LENGTH-2);i++) {
        cs = cs + command[i];
      }
#ifdef DEBUG
      Serial.write('c');Serial.println(cs);
#endif
      commandComplete = (cs == command[COMMAND_LENGTH-2]);
      cmdPos = 0;
    }
  }
#endif

  if (anim.run()) {
#ifdef BT
    if (commandComplete && command[0] == CMD_SETAP) {
      if ((command[1] < ANIMS) && (command[2] < PALS)) {
          anim.setAnim(command[1]);
          anim.setPalette(pals[command[2]]);
          bt.write(command, 3);
          EffectAutoChangeTimer.start();
      }
    }

    if (commandComplete && command[0] == CMD_MAGIC) {
      animInd = 7; //todo: why 7? (remove magik constant)
      anim.setAnim(animInd);
      anim.doSetUp();
      EffectAutoChangeTimer.start();
      bt.print('!');
    }
    commandComplete = false;
#endif
  }

#ifdef BUTTON_NEXT_EFF
  if (btnNextEff.flags.click) {
    // random pallete
    int prevPalInd = paletteInd;
    while (prevPalInd == paletteInd) paletteInd = random(PALS);
    // iterate animate
    animInd++;
    if (animInd >= ANIMS)
      animInd = 0;
    // apply
    anim.setAnim(animInd);
    anim.setPeriod(random(20, 40));
    anim.setPalette(pals[paletteInd]);
    anim.doSetUp();

    disableAutoChangeEffects = true;
    DetectTripleFastClickTimer.startOnce();
    DetectTripleFastClickCounter++;
  }

  if (DetectTripleFastClickTimer.run()) {
    if (DetectTripleFastClickCounter == 3) {
      disableAutoChangeEffects = false;
      // force change effect
      needChange = true;
    }
    DetectTripleFastClickCounter = 0;
  }
#endif // BUTTON_NEXT_EFF

  // auto change effect
  if (needChange && ! disableAutoChangeEffects) {
    EffectAutoChangeTimer.start();

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
#ifdef BT
    bt.print(F(">"));bt.print(animInd);bt.print(F("\t"));bt.println(paletteInd);
#ifndef BTHS
    bt.listen();
#endif
#endif
  }


}

