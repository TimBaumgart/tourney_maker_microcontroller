#include "ColorCharacteristic.h"
#include <BLE2902.h>
#include "scoreboard.h"

#define COLOR_CHARACTERISTIC_UUID "30087a03-fad4-4c3a-a4a8-56eabcbb8d79"

class ColorCallbacks : public BLECharacteristicCallbacks
{
    TourneyMakerScoreboard *scoreboard;

    void onWrite(BLECharacteristic *c) override
    {
        std::string value = c->getValue();
        scoreboard->colorReceived(value);
    }

public:
    explicit ColorCallbacks(TourneyMakerScoreboard *sb)
        : scoreboard(sb) {}
};

ColorCharacteristic::ColorCharacteristic(
    BLEService *service,
    TourneyMakerScoreboard *scoreboard)
{
    characteristic = service->createCharacteristic(
        COLOR_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_WRITE);

    characteristic->addDescriptor(new BLE2902());
    characteristic->setCallbacks(new ColorCallbacks(scoreboard));
}
