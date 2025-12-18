#pragma once

#include <BLECharacteristic.h>
#include <BLEService.h>
#include <string>

class TourneyMakerScoreboard;

class ColorCharacteristic
{
public:
    ColorCharacteristic(
        BLEService *service,
        TourneyMakerScoreboard *scoreboard);

private:
    BLECharacteristic *characteristic;
};
