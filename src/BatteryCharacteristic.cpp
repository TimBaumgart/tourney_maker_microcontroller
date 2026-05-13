#include "BatteryCharacteristic.h"
#include <BLE2902.h>
#include "scoreboard.h"

#define BATTERY_CHARACTERISTIC_UUID "e72a4956-867b-4a2b-8df1-521401ebfd74"

class BatteryCallbacks : public BLECharacteristicCallbacks
{
    TourneyMakerScoreboard *scoreboard;

    void onRead(BLECharacteristic *c) override
    {
        std::string value = c->getValue();
        uint8_t percentage = scoreboard->readBatteryPercentage();
        c->setValue(&percentage, 1);
    }

public:
    explicit BatteryCallbacks(TourneyMakerScoreboard *sb)
        : scoreboard(sb) {}
};

BatteryCharacteristic::BatteryCharacteristic(
    BLEService *service,
    TourneyMakerScoreboard *scoreboard)
{
    characteristic = service->createCharacteristic(
        BATTERY_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ);

    characteristic->addDescriptor(new BLE2902());
    characteristic->setCallbacks(new BatteryCallbacks(scoreboard));
}
