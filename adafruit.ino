#include <Arduino.h>

#include "anim_test.h"
#include "anim_start.h"

Anim * a[2];

void setup() {
  a[0] = new AnimTest();
  a[1] = new AnimStart();
  a[1]->setPeriod(10);
}



void loop() {

  a[1]->run();
}

