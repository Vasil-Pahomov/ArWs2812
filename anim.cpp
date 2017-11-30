#include <Arduino.h>
#include "ExtNeoPixel.h"
#include "color.h"
#include "palette.h"
#include "anim.h"
#include "brightness.h"

Anim::Anim() 
{
    if (leds == 0) {
        // allocate and clear leds array
        leds = (Color *)malloc(3*LEDS);
        memset(leds, 0, 3*LEDS);        
    }
    pixels.begin();
    
    pixels.show(); // Устанавливаем все светодиоды в состояние "Выключено"
    nextms = millis();
}

void Anim::setPeriod(byte period) {
    this->period = period;
}

void Anim::setPalette(Palette * pal) {
    this->palette = pal;
}

void Anim::run()
{    
    if ( millis()<=nextms) return;
    nextms=millis() + period;
    runImpl();
    for(int i=0; i<LEDS; i++)
        pixels.setPixelColor(i, pixels.Color(BRI[leds[i].r], BRI[leds[i].g], BRI[leds[i].b]));
  
    pixels.show();
    
}

void Anim::setUp()
{}

Adafruit_NeoPixel Anim::pixels = Adafruit_NeoPixel(LEDS, PIN, NEO_GRB + NEO_KHZ800); 
Color * Anim::leds = 0;
