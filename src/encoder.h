#ifndef ENCODER_H
#define ENCODER_H

#include <Arduino.h>
#include <ESP32Encoder.h>

struct rotaryencoder {
  int pinA;
  int pinB;
  ESP32Encoder encoder;
  int pulse;
  int prevPulse;
  int nbPulseToSimulate;
  signed char axisValue;
};

void initEncoder(rotaryencoder* enc) {
  enc->encoder.attachFullQuad(enc->pinA, enc->pinB);
  enc->encoder.clearCount();
}

void readEncoder(rotaryencoder* enc) {
  int pulse = enc->encoder.getCount();

  // if the encoder is on en "click position", ie multi 4
  if ( (pulse % 4) == 0 ) {
    enc->prevPulse = enc->pulse;        // we store the previous pulse to detect change

    enc->pulse = pulse;
    
    int valueAxis = enc->pulse % 80;    // Axis position calcul : 1 tuen is 80 pulse
    if (valueAxis < 0) valueAxis += 80; // if modulo is negatif, add 80 to translate in positif
    enc->axisValue = (signed char) map(valueAxis, 0, 80, -127, 127); // convert to axis resolution

    int delta = (enc->pulse - enc->prevPulse) / 4; // ccompute the nb click to increment the nb pulse sim left
    enc->nbPulseToSimulate += delta;
  }

}

#endif