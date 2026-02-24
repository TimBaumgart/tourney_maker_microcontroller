#ifndef BUZZER_H
#define BUZZER_H

#include <Arduino.h>

class Buzzer
{
private:
    int pin; // GPIO pin for LED
    unsigned long duration;

public:
    // Constructor: specify pin and optional blink interval
    Buzzer(int pin, unsigned long duration = 500);

    void buzz();
    void buzzFor(unsigned long duration);
};

#endif
