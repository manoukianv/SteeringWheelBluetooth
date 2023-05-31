#ifndef BATTERY_H
#define BATTERY_H

#include <Arduino.h>
#include "esp_adc_cal.h"

#if defined(CONFIG_ARDUHAL_ESP_LOG)
  #include "esp32-hal-log.h"
  #define LOG_TAG ""
#else
  #include "esp_log.h"
#endif

struct battery {
  int pin;
  uint32_t maxVoltage;
  uint32_t minVoltage;
  float rGND;
  float rVCC;
  uint8_t state;
  uint8_t prevState;
};

esp_adc_cal_characteristics_t adc_chars;

uint32_t readADC_Cal(int ADC_Raw)
{
  return(esp_adc_cal_raw_to_voltage(ADC_Raw, &adc_chars));
}

void initBattery(battery* b) {
  pinMode(b->pin, INPUT);
}

void readBattery(battery* b) {
  const static int n = 16;
  const static int resolution = 4096;
  const static float maxVoltage = 3.3;

  int raw = 0;
  for (int i = 0; i < n; i++) {
    raw += readADC_Cal(analogRead(b->pin));    
  }

  float avg = float(raw) / float(n);
  float batteryVoltage = avg * (b->rGND + b->rVCC) / b-> rGND;

  int val = map(batteryVoltage, b->minVoltage, b->maxVoltage, 0, 100);
  uint8_t level = 0;
  if (val>100) {
    level = 100;
  } else if (val<0) {
    level = 0;
  } else {
    level = val;
  }

  b->prevState = b->state;
  b->state = level;

  log_d("battery adc_avg:%.2f batteryVoltage:%.2f  min/max:%lu/%lu  batterylevel:%d",avg,batteryVoltage,
    b->minVoltage, b->maxVoltage, level);
}

#endif