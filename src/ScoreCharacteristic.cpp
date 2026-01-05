#include "ScoreCharacteristic.h"
#include <BLE2902.h>
#include "scoreboard.h"

#define SCORE_CHARACTERISTIC_UUID "2a08da31-68a4-4047-92af-95145bb7bd07"

class ScoreCallbacks : public BLECharacteristicCallbacks
{
    TourneyMakerScoreboard *scoreboard;

    void onWrite(BLECharacteristic *c) override
    {
        std::string value = c->getValue();
        if (value.length() != 2)
        {
            // Ignore CCCD writes or invalid data
            return;
        }

        scoreboard->scoreReceived(value);
    }

    void onRead(BLECharacteristic *c) override
    {
        // Prepare the current score
        uint8_t data[2] = {scoreboard->getScore1(), scoreboard->getScore2()};
        c->setValue(data, sizeof(data));
    }

public:
    explicit ScoreCallbacks(TourneyMakerScoreboard *sb)
        : scoreboard(sb) {}
};

ScoreCharacteristic::ScoreCharacteristic(
    BLEService *service,
    TourneyMakerScoreboard *scoreboard)
{
    characteristic = service->createCharacteristic(
        SCORE_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE |
            BLECharacteristic::PROPERTY_NOTIFY);

    characteristic->addDescriptor(new BLE2902());
    characteristic->setCallbacks(new ScoreCallbacks(scoreboard));
}

void ScoreCharacteristic::notify(uint8_t score1, uint8_t score2)
{
    uint8_t data[2] = {score1, score2};
    characteristic->setValue(data, sizeof(data));
    characteristic->notify();
}
