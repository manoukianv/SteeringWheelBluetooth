#include <Arduino.h>
#include <BleGamepad.h>
#include "button.h" 
#include "battery.h"

BleGamepad bleGamepad;

button btn0 = { .pin = 23, .reverse = false };
button btn1 = { .pin = 22, .reverse = false  };
button btn2 = { .pin = 21, .reverse = false  };
button btn3 = { .pin = 19, .reverse = false  };
button btn4 = { .pin = 18, .reverse = true  };
button btn5 = { .pin = 5, .reverse = true  };
button btn6 = { .pin = 17, .reverse = false  };
button btn7 = { .pin = 16, .reverse = false  };
button btn8 = { .pin = 4, .reverse = false };
button btn9 = { .pin = 0, .reverse = false  };
button* btn[] = { &btn0, &btn1, &btn2, &btn3, &btn4, &btn5, &btn6, &btn7, &btn8, &btn9 };
const int N_BUTTONS = sizeof(btn) / sizeof(btn[0]);

battery bat = { .pin = 35, .maxVoltage = 8.2, .minVoltage = 7 };
void setup() {
  Serial.begin(115200);
  esp_log_level_set("*", ESP_LOG_VERBOSE);

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

  ESP_LOGV(LOG_TAG, "loop.. \n");
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