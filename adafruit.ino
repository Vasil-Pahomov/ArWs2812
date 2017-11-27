#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include "color.h"

#define PIN 2 // номер порта к которому подключен модуль
#define LEDS 100 // количество светодиодов 


Adafruit_NeoPixel pixels = Adafruit_NeoPixel(LEDS, PIN, NEO_GRB + NEO_KHZ800); 
AlaColor *leds; 


long ms;//последнее значение millis() для отслеживания скорости анимации
byte phase_c = 0; //счётчик циклов внутри фазы
byte phase=0;
byte anim=0;

byte step_ms = 10; 
byte phase_t = 4;

void setup() {
  pixels.begin();
  pixels.show(); // Устанавливаем все светодиоды в состояние "Выключено"

  // allocate and clear leds array
  leds = (AlaColor *)malloc(3*LEDS);
  memset(leds, 0, 3*LEDS);
  ms=millis()+100;
  pinMode(LED_BUILTIN, OUTPUT);
}



void loop() {

  if ( (millis()-ms) < step_ms) return;
  ms=millis();

  switch (anim) {
    case 0: //startup animation
      if (phase_c == 0) {
        if (phase < LEDS) {
          leds[phase].r = 255;
          leds[phase].g = 255;
          leds[phase].b = 255;
        } else if (phase == LEDS) {
          for(int i=0; i<LEDS; i++) {
            leds[i].r = 255;
            leds[i].g = 255;
            leds[i].b = 255;
          }
        }
      }

      if (phase <= LEDS) {
        for(int i=0; i<LEDS; i++) {
          leds[i].fade(5);
        }
      } else {
        for(int i=0; i<LEDS; i++) {
          leds[i].fade3(random(0,3), random(0,3), random(0,3));
        }
      }
      
      break;
      
    case 1:
      break;
    default:
      anim = 0;
      break;
  }

  
  for(int i=0; i<LEDS; i++)
    pixels.setPixelColor(i, pixels.Color(leds[i].r, leds[i].g, leds[i].b));
  
  pixels.show();

  if ( phase_c++ > phase_t) {
    phase++;
    phase_c = 0;
    if (phase == 0)
    {
      anim++; 
    }
  }

  digitalWrite(LED_BUILTIN, phase & 1);
}

