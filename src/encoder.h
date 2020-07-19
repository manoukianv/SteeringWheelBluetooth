#ifndef ENCODER_H
#define ENCODER_H

#include <Arduino.h>
#include <ESP32Encoder.h>

struct rotaryencoder {
  int pinA;
  int pinB;
  ESP32Encoder encoder;
  signed char state;
  signed char prevState;
};

void initEncoder(rotaryencoder* enc) {
  enc->encoder.attachFullQuad(enc->pinA, enc->pinB);
  enc->encoder.clearCount();
}

void readEncoder(rotaryencoder* enc) {
  enc->prevState = enc->state;
  enc->state = enc->encoder.getCount();

  int valuex = (enc->encoder.getCount() % 80);
  if (valuex < 0) valuex += 80;
  enc->state = (signed char) map(valuex, 0, 80, -127, 127);
}

#endif