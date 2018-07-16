#include <SoftwareSerial.h>
#include "anim.h"
#include "color.h"
#include "palette.h"
#include "commands.h"

extern bool commandComplete;
extern byte command[]; 

//width of "spot" controlled by phone, in leds
#define SPOT_WIDTH 10

void Anim::animBT_SetUp() {
    leds[10] = sparkleColor; 
}

void Anim::animBT_Run() {
  Color col;
  int pos = -LEDS;
  if (commandComplete && command[0] == CMD_MPOS) {
     pos = command[1]*LEDS/256;
     byte cin = command[2];
     if (cin <= 63) {
       col.r = cin*4;
       col.g = (63-cin)*4;
       col.b = 0;
     } else if (cin <= 127) {
       col.r = 255;
       col.g = (cin - 64)*4;
       col.b = col.g;
     } else if (cin <= 191) {
       col.r = (191 - cin) * 4;
       col.g = col.r;
       col.b = 255;
     } else {
      col.r = 0;
      col.g = (cin - 192) * 4;
      col.b = (255 - cin) * 4;
     }
     Serial.print('M');Serial.print(pos);Serial.print('-');Serial.println(cin);
  }

  
  for (int i=0;i<LEDS;i++) {
      leds[i].fade(5);
      if ( ((i-pos) >=0) && ((i-pos) < SPOT_WIDTH)) {
        leds[i] = col;
      }
   }
}

