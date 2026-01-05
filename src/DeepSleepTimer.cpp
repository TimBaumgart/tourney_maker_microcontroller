#include "DeepSleepTimer.h"

DeepSleepTimer::DeepSleepTimer(gpio_num_t wakeUpPin, unsigned long timeoutMs)
{
    this->timeoutMs = timeoutMs;
    enabled = false;
    startTime = 0;
    esp_sleep_enable_ext0_wakeup(wakeUpPin, 1);
}

void DeepSleepTimer::start()
{
    startTime = millis();
    enabled = true;
}

void DeepSleepTimer::stop()
{
    enabled = false;
}

void DeepSleepTimer::reset()
{
    if (enabled)
    {
        startTime = millis();
    }
}

void DeepSleepTimer::update()
{
    if (!enabled)
        return;

    unsigned long now = millis();
    if (now - startTime >= timeoutMs)
    {
        // Prepare for deep sleep
        Serial.print("deep sleep");
        esp_deep_sleep_start();
    }
}
