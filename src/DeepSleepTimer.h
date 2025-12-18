#ifndef DEEPSLEEPTIMER_H
#define DEEPSLEEPTIMER_H

#include <Arduino.h>

class DeepSleepTimer
{
private:
    unsigned long timeoutMs; // Time before sleep
    unsigned long startTime; // When timer started
    bool enabled;

public:
    // timeoutMinutes
    DeepSleepTimer(gpio_num_t wakeUpPin, unsigned long timeoutMinutes);

    // Enable the deep sleep timer
    void start();

    // Disable the deep sleep timer
    void stop();

    // Reset countdown (call on activity)
    void reset();

    // Call frequently in loop()
    void update();
};

#endif
