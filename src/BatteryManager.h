#ifndef BATTERYMANAGER_H
#define BATTERYMANAGER_H

#include <Arduino.h>

class BatteryManager
{
private:
    int pin; // Blinking enabled flag

public:
    // Constructor: specify pin and optional blink interval
    BatteryManager(int pin);

    // get battery level
    uint16_t readRawValue();
    uint8_t readPercentage();
};

#endif
