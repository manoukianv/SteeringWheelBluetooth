#ifndef ESP32_BLE_GAMEPAD_H
#define ESP32_BLE_GAMEPAD_H
#include "sdkconfig.h"
#if defined(CONFIG_BT_ENABLED)

#include "BleConnectionStatus.h"
#include "BLEHIDDevice.h"
#include "BLECharacteristic.h"

#define DPAD_CENTERED 	0
#define DPAD_UP 		1
#define DPAD_UP_RIGHT 	2
#define DPAD_RIGHT 		3
#define DPAD_DOWN_RIGHT 4
#define DPAD_DOWN 		5
#define DPAD_DOWN_LEFT 	6
#define DPAD_LEFT 		7
#define DPAD_UP_LEFT 	8

class BleGamepad {
private:
  uint32_t _buttons;
  BleConnectionStatus* connectionStatus;
  BLEHIDDevice* hid;
  BLECharacteristic* inputGamepad;
  void buttons(uint32_t b);
  void rawAction(uint8_t msg[], char msgSize);
  static void taskServer(void* pvParameter);
public:
  BleGamepad(std::string deviceName = "ESP32 BLE Gamepad", std::string deviceManufacturer = "Espressif", uint8_t batteryLevel = 0);
  void begin(void);
  void end(void);
  void setAxes(signed char x, signed char y, signed char z = 0, signed char rZ = 0, char rX = 0, char rY = 0, signed char hat = 0);
  void press(int buttonNb = 1);   // press BUTTON_1 by default
  void release(int buttonNb = 1); // release BUTTON_1 by default
  bool isPressed(int buttonNb = 1); // check BUTTON_1 by default
  bool isConnected(void);
  void setBatteryLevel(uint8_t level);
  uint8_t batteryLevel;
  std::string deviceManufacturer;
  std::string deviceName;
protected:
  virtual void onStarted(BLEServer *pServer) { };
};

#endif // CONFIG_BT_ENABLED
#endif // ESP32_BLE_GAMEPAD_H
