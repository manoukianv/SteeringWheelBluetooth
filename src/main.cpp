#include <Arduino.h>
#include <BleGamepad.h>
#include "button.h" 
#include "battery.h"
#include "encoder.h"

BleGamepad bleGamepad ("Wireless Wheel", "Vincent Manoukian", 0);

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

rotaryencoder enc1 = { .pinA = 2, .pinB = 15};
rotaryencoder enc2 = { .pinA = 14, .pinB = 12};
rotaryencoder* enc[] = { &enc1, &enc2 };
const int N_ENCODERS = sizeof(enc) / sizeof(enc[0]);

battery bat = { .pin = 35, .maxVoltage = 8.2, .minVoltage = 7 };

void setup() {
  Serial.begin(115200);
  esp_log_level_set("*", ESP_LOG_DEBUG);

  initBattery(&bat);

  for (int i = 0; i < N_BUTTONS; i++) {
    initButton(btn[i]);
    readButton(btn[i]);
  }

  ESP32Encoder::useInternalWeakPullResistors=UP;

  initEncoder(&enc1);
  initEncoder(&enc2);

  ESP_LOGI(LOG_TAG, "loop...");
  bleGamepad.begin();
}

int nbCount = 0;

void loop() {

  nbCount++;

  ESP_LOGV(LOG_TAG, "loop...");
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
        ESP_LOGD(LOG_TAG, "button %d : %d", i, btn[i]->state);
        if (btn[i]->state) {
          bleGamepad.press(i + 1);
        } else {
          bleGamepad.release(i + 1);
        }
      }
    }

    bool encoderChange = false;
    for (int i = 0; i < N_ENCODERS; i++) {
      readEncoder(enc[i]);
      if (enc[i]->state != enc[i]->prevState) {
        ESP_LOGD(LOG_TAG, "encoder %d : %d", i, enc[i]->state);
        encoderChange |= true;
      }
    }
    if (encoderChange) {
      bleGamepad.setAxes(enc1.state, enc2.state, 0, 0, 0, 0, 0);
    }

    delay(5);
  }

}