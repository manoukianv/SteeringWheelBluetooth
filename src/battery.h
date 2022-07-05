#ifndef BATTERY_H
#define BATTERY_H

#include <Arduino.h>

#if defined(CONFIG_ARDUHAL_ESP_LOG)
  #include "esp32-hal-log.h"
  #define LOG_TAG ""
#else
  #include "esp_log.h"
  static const char* LOG_TAG = "BLEDevice";
#endif

struct battery {
  int pin;
  float maxVoltage;
  float minVoltage;
  float rGND;
  float rVCC;
  float state;
  float prevState;
};

void initBattery(battery* b) {
  analogSetAttenuation(ADC_11db);
  pinMode(b->pin, ANALOG);
}

void readBattery(battery* b) {
  const static int n = 16;
  const static int resolution = 4096;
  const static float maxVoltage = 2.4;

  int raw = 0;
  for (int i = 0; i < n; i++) {
    raw += analogRead(b->pin);    
  }

  float avg = float(raw) / float(n);
  float adcVoltage = avg * maxVoltage / resolution;
  float batteryVoltage = adcVoltage * (b->rGND + b->rVCC) / b-> rGND;
  log_d( "level battery %.2fV (%.2fV/%.2f)!", batteryVoltage, adcVoltage, avg);
  float val = map(batteryVoltage, b->minVoltage, b->maxVoltage, 0, 100);
  val = roundf(val * 100) / 100;
  b->prevState = b->state;
  b->state = val;
}

#endif