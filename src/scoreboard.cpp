
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "esp_pm.h"
#include "ScoreboardPrefs.h"
#include "Scoreboard.h"

BLEServer *pServer = NULL;

#define SERVICE_UUID "621c7b43-a755-4456-b3e5-946a58bf20d9"

TourneyMakerScoreboard::TourneyMakerScoreboard(std::string id)
{
    this->id = id;
    this->score1 = ScoreboardPrefs::getScore1Or0();
    this->score2 = ScoreboardPrefs::getScore2Or0();
}

class MyServerCallbacks : public BLEServerCallbacks
{
    TourneyMakerScoreboard *scoreboard;

public:
    MyServerCallbacks(TourneyMakerScoreboard *scoreboard)
    {
        this->scoreboard = scoreboard;
    }
    void onConnect(BLEServer *pServer)
    {
        scoreboard->connected();
    };

    void onDisconnect(BLEServer *pServer)
    {
        scoreboard->disconnected();
        delay(500); // give the bluetooth stack the chance to get things ready
        scoreboard->startAdvertising();
    }
};

TourneyMakerScoreboard *TourneyMakerScoreboard::setup()
{
    std::string globalFieldId = ScoreboardPrefs::getGlobalFieldId();
    Serial.println(("Initializing scoreboard " + globalFieldId + "...").c_str());

    TourneyMakerScoreboard *scoreboard = new TourneyMakerScoreboard(globalFieldId);

    // Create the BLE Device
    BLEDevice::init(globalFieldId);

    // Create the BLE Server
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks(scoreboard));

    // Create the BLE Service
    BLEService *pService = pServer->createService(SERVICE_UUID);

    scoreboard->idCharacteristic = new IdCharacteristic(pService);
    scoreboard->scoreCharacteristic = new ScoreCharacteristic(pService, scoreboard);
    scoreboard->colorCharacteristic = new ColorCharacteristic(pService, scoreboard);

    // Start the service
    pService->start();

    // Start advertising
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06); // functions that help with iPhone connections issue
    pAdvertising->setMinPreferred(0x12);

    return scoreboard;
}

void TourneyMakerScoreboard::connected()
{
    this->deviceConnected = true;
    Serial.println("connected");
    scoreboardStatusCallback->onConnected();
}

void TourneyMakerScoreboard::disconnected()
{
    this->deviceConnected = false;
    Serial.println("disconnected");
    scoreboardStatusCallback->onDisconnected();
}

void TourneyMakerScoreboard::startAdvertising()
{
    BLEDevice::startAdvertising();
    Serial.println("Start advertising...");
    scoreboardStatusCallback->onStartAdvertisement();
}

void TourneyMakerScoreboard::bumpScore(uint8_t diff1, uint8_t diff2)
{
    this->setScore(this->score1 + diff1, this->score2 + diff2);
}

void TourneyMakerScoreboard::setScore(uint8_t score1, uint8_t score2)
{
    this->score1 = score1;
    this->score2 = score2;

    ScoreboardPrefs::setScore(score1, score2);

    if (!deviceConnected)
    {
        return;
    }

    scoreCharacteristic->notify(score1, score2);
    Serial.println("new score sent: " + String(this->score1) + ":" + String(this->score2));
}

void TourneyMakerScoreboard::scoreReceived(std::string value)
{
    if (value.length() < 2)
    {
        return;
    }

    uint8_t score1 = value[0];
    uint8_t score2 = value[1];

    this->score1 = score1;
    this->score2 = score2;
    Serial.println("new score received: " + String(this->score1) + ":" + String(this->score2));
    scoreboardChangedCallback->onScoreReceived(score1, score2);
}

void TourneyMakerScoreboard::colorReceived(std::string value)
{
    uint8_t r1 = value[0];
    uint8_t g1 = value[1];
    uint8_t b1 = value[2];
    uint8_t r2 = value[3];
    uint8_t g2 = value[4];
    uint8_t b2 = value[5];
    uint32_t color1 = (r1 << 16) | (g1 << 8) | b1;
    uint32_t color2 = (r2 << 16) | (g2 << 8) | b2;
    Serial.println("new color received: " + String(color1) + " - " + String(color2));
    scoreboardChangedCallback->onColorReceived(color1, color2);
}

uint8_t TourneyMakerScoreboard::getScore1()
{
    return score1;
}

uint8_t TourneyMakerScoreboard::getScore2()
{
    return score2;
}
