#pragma once

#include <BLECharacteristic.h>
#include <BLEService.h>
#include <string>

class TourneyMakerScoreboard;

class ScoreCharacteristic
{
public:
    ScoreCharacteristic(
        BLEService *service,
        TourneyMakerScoreboard *scoreboard);

    void notify(uint8_t score1, uint8_t score2);

private:
    BLECharacteristic *characteristic;
};