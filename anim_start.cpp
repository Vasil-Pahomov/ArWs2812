#include <Arduino.h>
#include "anim.h"

unsigned long ppms;

void Anim::animStart_SetUp() {
    Serial.print(F("(start)"));
    phase = 0;
    ppms = millis();
}

void Anim::animStart_Run() {

    if ( millis() < (ppms + 10000)) return;
    ppms = millis();
    Color c;
    phase++;
    switch (phase) {
        case 1: 
            c = Color(255,0,0);
        break;
        case 2: 
            c = Color(0,255,0);
        break;
        case 3: 
            c = Color(0,0,255);
        break;
        case 4: 
            c = Color(255,255,255);
        break;
        case 6: 
            phase = 0;
        break;
    }
    for(int i=0; i<LEDS; i++) {
        leds[i] = c;
    }        
}
