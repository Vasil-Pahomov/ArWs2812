#include <Arduino.h>

#include "anim_test.h"

AnimTest a = AnimTest();

void setup() {
  a.setPeriod(500);
}



void loop() {

  a.run();
}

