#include <Arduino.h>
#include <BleGamepad.h>
#include "button.h" 
#include "battery.h"

BleGamepad bleGamepad;

button btn0 = { .pin = 23 };
button btn1 = { .pin = 22 };
button btn2 = { .pin = 21 };
button btn3 = { .pin = 19 };
button btn4 = { .pin = 18 };
button btn5 = { .pin = 5 };
button btn6 = { .pin = 17 };
button btn7 = { .pin = 16 };
button btn8 = { .pin = 4 };
button btn9 = { .pin = 0 };
button* btn[] = { &btn0, &btn1, &btn2, &btn3, &btn4, &btn5, &btn6, &btn7, &btn8, &btn9 };
const int N_BUTTONS = sizeof(btn) / sizeof(btn[0]);

battery bat = { .pin = 35, .maxVoltage = 8.2, .minVoltage = 7 };
void setup() {
  Serial.begin(115200);

  initBattery(&bat);

  for (int i = 0; i < N_BUTTONS; i++) {
    initButton(btn[i]);
    readButton(btn[i]);
  }

  Serial.println("Starting BLE work!");
  bleGamepad.begin();
}

int nbCount = 0;

void loop() {

  nbCount++;


  if(bleGamepad.isConnected()) {

    if (nbCount > (2000 / 5)) {
      readBattery(&bat);
      nbCount = 0;
      if (bat.state != bat.prevState) {
        if (bat.state >= 0) {
          bleGamepad.setBatteryLevel(bat.state);
        } else {
          bleGamepad.setBatteryLevel(0);
        }
        
      }
    }

    for (int i = 0; i < N_BUTTONS; i++) {
      readButton(btn[i]);
      if (btn[i]->state != btn[i]->prevState) {
        if (btn[i]->state) {
          bleGamepad.press(1 << i);
        } else {
          bleGamepad.release(1 << i);
        }
      }
    }

    delay(5);
  }

}