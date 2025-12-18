#ifndef BLINKINGLED_H
#define BLINKINGLED_H

#include <Arduino.h>

class BlinkingLED
{
private:
    int pin;                  // GPIO pin for LED
    unsigned long interval;   // Blink interval in milliseconds
    unsigned long lastToggle; // Last time LED toggled
    bool state;               // Current LED state
    bool enabled;             // Blinking enabled flag

public:
    // Constructor: specify pin and optional blink interval
    BlinkingLED(int ledPin, unsigned long blinkInterval = 500);

    // Start blinking
    void start();

    // Stop blinking (LED turns on/off)
    void stop(boolean on);

    // Update LED state, call frequently in loop()
    void update();

    // Change blink interval dynamically
    void setInterval(unsigned long blinkInterval);
};

#endif
