#include "Bitling.h"

#define NUMBER_OF_BITLINGS 8

Bitling BITLINGS[NUMBER_OF_BITLINGS];

const int READ_RESOLUTION = 16; // bits

void setup() {
  Serial.begin(9600);

  analogReadResolution(READ_RESOLUTION);

  for (int i = 0; i < NUMBER_OF_BITLINGS; i++) {
    BITLINGS[i].setup();
  }

  lastTime = micros();
}

void loop() {
  unsigned long currentTime = micros();
  unsigned long delta = currentTime - lastTime;
  float deltaTime = delta / 1000000.0f;
  lastTime = currentTime;

  for (int i = 0; i < NUMBER_OF_BITLINGS; i++) {
    BITLINGS[i].setup();
  }


  delay(PULSE_TIME);// - delta / 1000);
}

