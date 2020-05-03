#include <Arduino.h>
#include "BleGamepad.h" 

BleGamepad bleGamepad;


void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");
  bleGamepad.begin();
}

void loop() {

  if(bleGamepad.isConnected()) {
    Serial.println("Press buttons 1 and 14. Move all axes to max. Set DPAD to down right.");
    bleGamepad.press(BUTTON_14);
    bleGamepad.press(BUTTON_1);
    bleGamepad.setAxes(127, 127, 127, 127, 127, 127, DPAD_DOWN_RIGHT);
    delay(500);

    Serial.println("Release button 14. Move all axes to min. Set DPAD to centred.");
    bleGamepad.release(BUTTON_14);
    bleGamepad.setAxes(-127, -127, -127, -127, -127, -127, DPAD_CENTERED);
    delay(500);
  }
  /*
  readBattery(&bat);
  if (bat.state != bat.prevState) {
    drawBattery(&bat, 10, 0);
  }

  for (int i = 0; i < N_BUTTONS; i++) {
    readButton(btn[i]);
  }
  
  for (int i = 0; i < N_JOYSTICK_AXES; i++) {
    readJoystick(axis[i]);
  }
  applyDeadzone(axis[0], axis[1], 20);

  bool stateChange = false;

  for (int i = 0; i < N_BUTTONS; i++) {
    if (btn[i]->state != btn[i]->prevState) {
      stateChange = true;
      //drawButton(btn[i], i, 0);
      Serial.printf("%lu\tbtn %i\t%i\n", millis(), i, btn[i]->state);
    }
  }

  for (int i = 0; i < N_JOYSTICK_AXES; i++) {
    if (axis[i]->state != axis[i]->prevState) {
      stateChange = true;
      //drawJoystick(axis[i], 4*i, 2);
      Serial.printf("%lu\taxis %i\t%i\n", millis(), i, axis[i]->state);
    }
  }

  // Send BLE update.
  if (connected && stateChange) {
    int8_t x = axis[0]->state;
    int8_t y = axis[1]->state;
    int8_t z = axis[2]->state;
    uint8_t b0 = 0, b1 = 0;
    for (int i = 0; i < N_BUTTONS; i++) {
      int index =  i / 8;
      int bitpos = i % 8;
      if (index == 0) { 
        b0 |= btn[i]->state << bitpos;
      } else {
        b1 |= btn[i]->state << bitpos;
      }
      //Serial.printf("%d-b[%d] << %d\n", i, index, bitpos);
    }
    uint8_t a[] = {(uint8_t)x, (uint8_t)y, (uint8_t)z, b0, b1};
    input->setValue(a, sizeof(a));
    input->notify();
  }

  delay(5);*/
}