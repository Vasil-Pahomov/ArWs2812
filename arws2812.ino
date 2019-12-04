#include <Adafruit_NeoPixel.h>
#define _SS_MAX_RX_BUFF 8 //lower SoftwareSerial's receive buffer to conserve some RAM
#include <SoftwareSerial.h>

#include "palette.h"
#include "anim.h"
#include "commands.h"
#include "small_timer.hpp"
#include "button_classes.hpp"

#define ANIMS 7 //number of animations
#define PALS 7 //number of palettes
#define INTERVAL 10000 //change interval, msec

#define BUTTON_NEXT_EFF

constexpr auto pinButtonNextEff = A0; // button pin for next effect
constexpr bool buttonNextEffInverse = true; // options: inverse "button for next effect"

//#define BTHS //whether to use hardware serial to communicate Bluetooth. Software serial is used otherwise
//#define DEBUG //if defined, debug data is output to hardware serial port. REMEMBER TO REMOVE this definition once BTHS is set

Palette * pals[PALS] = {&PalRgb, &PalRainbow, &PalRainbowStripe, &PalParty, &PalHeat, &PalFire, &PalIceBlue};

Anim anim = Anim();

#ifndef BTHS
//11 and 12 are RX (from BT) and TX (to BT) pin numbers
SoftwareSerial bt(11,12);
#else
#define bt Serial
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
#ifdef DEBUG
  Serial.begin(9600);
  Serial.print(F("RAM="));Serial.println(freeRam());
#endif
  pixels.begin();
  bt.begin(9600);
  randomSeed(analogRead(0)*analogRead(1));
  anim.setAnim(animInd);
  anim.setPeriod(20);
  anim.setPalette(pals[0]);
  anim.doSetUp();
#ifndef BTHS
  bt.listen();
#endif

#ifdef BUTTON_NEXT_EFF
  pinMode(pinButtonNextEff, INPUT_PULLUP);
#endif // BUTTON_NEXT_EFF
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

#ifdef BUTTON_NEXT_EFF
  btnNextEff.run(digitalRead(pinButtonNextEff));
#endif // BUTTON_NEXT_EFF


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

  if (anim.run()) {
    if (commandComplete && command[0] == CMD_SETAP) {
      if ((command[1] < ANIMS) && (command[2] < PALS)) {
          anim.setAnim(command[1]);
          anim.setPalette(pals[command[2]]);
          bt.write(command, 3);
          ms = millis() + INTERVAL;
      }
    }

    if (commandComplete && command[0] == CMD_MAGIC) {
      animInd = 7; //todo: why 7? (remove magik constant)
      anim.setAnim(animInd);
      anim.doSetUp();
      //todo: why 60000? (remove magik constant)
      ms = millis() + 60000;
      bt.print('!');
    }
    commandComplete = false;
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
      //todo: add abstract the change mechanism
      // force change effect
      ms = millis();
    }
    DetectTripleFastClickCounter = 0;
  }
#endif // BUTTON_NEXT_EFF

  // auto change effect
  if ((millis() > ms) && ( ! disableAutoChangeEffects)) {
    //TODO: rollover bug. after 49 days uptime this code will be broken
    //      (correct code in: https://www.arduino.cc/en/tutorial/BlinkWithoutDelay and https://arduino.stackexchange.com/questions/12587/how-can-i-handle-the-millis-rollover )
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
#ifndef BTHS
    bt.listen();
#endif
  }


}

