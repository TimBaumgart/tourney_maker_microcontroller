#pragma once

#include <BLECharacteristic.h>
#include <BLEService.h>
#include <string>

class TourneyMakerScoreboard;

class BatteryCharacteristic
{
public:
    BatteryCharacteristic(
        BLEService *service,
        TourneyMakerScoreboard *scoreboard);

private:
    BLECharacteristic *characteristic;
};
