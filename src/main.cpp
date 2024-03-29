#include <Arduino.h>
#include <BleGamepad.h>
#include "button.h" 
#include "battery.h"
#include "encoder.h"

#define ENCODER_PRESS_TIME 100
#define ENC_SEND_BUTTON 1
#define ENC_SEND_AXIS 1

BleGamepad bleGamepad ("Wireless Wheel", "Vincent Manoukian", 0);

button btn0 = { .pin = 23, .reverse = false };
button btn1 = { .pin = 22, .reverse = false  };
button btn2 = { .pin = 21, .reverse = false  };
button btn3 = { .pin = 19, .reverse = false  };
button btn4 = { .pin = 18, .reverse = false  };
button btn5 = { .pin = 5, .reverse = false  };
button btn6 = { .pin = 17, .reverse = false  };
button btn7 = { .pin = 16, .reverse = false  };
button btn8 = { .pin = 4, .reverse = false };
button btn9 = { .pin = 0, .reverse = false  };
button* btn[] = { &btn0, &btn1, &btn2, &btn3, &btn4, &btn5, &btn6, &btn7, &btn8, &btn9 };
unsigned long lastTimeButtonPush[8];
const int N_BUTTONS = sizeof(btn) / sizeof(btn[0]);

rotaryencoder enc1 = { .pinA = 2, .pinB = 15};
rotaryencoder enc2 = { .pinA = 14, .pinB = 12};
rotaryencoder* enc[] = { &enc1, &enc2 };
const int N_ENCODERS = sizeof(enc) / sizeof(enc[0]);

battery bat = { .pin = 35, .maxVoltage = 8.2, .minVoltage = 7, .rGND = 18, .rVCC = 43.0 };

void setup() {
  Serial.begin(115200);

  initBattery(&bat);

  for (int i = 0; i < N_BUTTONS; i++) {
    initButton(btn[i]);
    readButton(btn[i]);
  }

  ESP32Encoder::useInternalWeakPullResistors=UP;

  initEncoder(&enc1);
  initEncoder(&enc2);

  log_i("loop...");
  BleGamepadConfiguration bleGamepadConfig;
  bleGamepadConfig.setControllerType(CONTROLLER_TYPE_GAMEPAD);
  bleGamepadConfig.setButtonCount(32);
  bleGamepadConfig.setHatSwitchCount(0);
  // if required more axis for encoder add true to enable it
  bleGamepadConfig.setWhichAxes(true, true, false, false, false, false, false, false);
  bleGamepadConfig.setAutoReport(false);

  bleGamepad.begin(&bleGamepadConfig);
}

int nbCount = 0;

void loop() {

  nbCount++;

  log_v("loop...");
  if(bleGamepad.isConnected()) {

    if (nbCount > 2000) {
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

    bool buttonChange = false;
    for (int i = 0; i < N_BUTTONS; i++) {
      readButton(btn[i]);
      if (btn[i]->state != btn[i]->prevState) {
        buttonChange = true;
        log_d("button %d : %d", i, btn[i]->state);
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
      if (enc[i]->pulse != enc[i]->prevPulse) {
        log_d("encoder %d : pulse(%d/%d) => Axis:%d - Delta%d", i, enc[i]->pulse, enc[i]->prevPulse, enc[i]->axisValue, enc[i]->nbPulseToSimulate);
        encoderChange = true;
      }
    }

    // if encoder simulate button do it
    if (ENC_SEND_BUTTON) {
      for (int i = 0; i < 8; i++) {
        if (bleGamepad.isPressed(25 + i) && ( (millis() - lastTimeButtonPush[i]) > ENCODER_PRESS_TIME )) {
          bleGamepad.release(25 + i);
          lastTimeButtonPush[i] = millis();
          encoderChange = true;
          log_d("encoder %d : release", i); 
        }
      }
      for (int i = 0; i < N_ENCODERS; i++) {
          int direction = enc[i]->nbPulseToSimulate >= 0 ? 1 : -1;
          int pinDirOffset = direction > 0 ? 0 : -1;
          int pin = (i*2) + 1 + pinDirOffset;

          if ( (enc[i]->nbPulseToSimulate != 0) && 
                !bleGamepad.isPressed(25 + pin) && 
              ( (millis() - lastTimeButtonPush[pin] ) > ENCODER_PRESS_TIME) ) {
            
            lastTimeButtonPush[pin] = millis();
            bleGamepad.press(25 + pin);
            enc[i]->nbPulseToSimulate -= direction;
            log_d("encoder %d : press", i); 
            encoderChange = true;
          } 
      }
    }
    // if the encoder set axis value, set this
    if (ENC_SEND_AXIS) {
      bleGamepad.setAxes(enc1.axisValue, enc2.axisValue);
    }

    // if button or encoder change => send HID
    if (encoderChange || buttonChange) {
      bleGamepad.sendReport();
    }

    delay(1);
  }

}