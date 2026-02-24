#include <Arduino.h>
#include <ScoreCharacteristic.h>
#include <ColorCharacteristic.h>
#include <IdCharacteristic.h>

class ScoreboardChangedCallback
{
public:
    virtual void onScoreChanged(uint8_t score1, uint8_t score2);
    virtual void onColorReceived(uint32_t color1, uint32_t color2);
};

class ScoreboardStatusCallback
{
public:
    virtual void onConnected();
    virtual void onDisconnected();
    virtual void onStartAdvertisement();
};

class TourneyMakerScoreboard
{
private:
    std::string id;
    uint8_t score1;
    uint8_t score2;
    IdCharacteristic *idCharacteristic;
    ScoreCharacteristic *scoreCharacteristic;
    ColorCharacteristic *colorCharacteristic;
    TourneyMakerScoreboard(std::string id);

public:
    bool deviceConnected = false;
    void connected();
    void disconnected();
    void startAdvertising();
    void bumpScore(int8_t diff1, int8_t diff2);
    void setScore(uint8_t score1, uint8_t score2);
    uint8_t getScore1();
    uint8_t getScore2();
    void scoreReceived(std::string value);
    void colorReceived(std::string value);
    ScoreboardChangedCallback *scoreboardChangedCallback;
    ScoreboardStatusCallback *scoreboardStatusCallback;
    static TourneyMakerScoreboard *setup();
    // void setScoreReceivedCallback(ScoreReceivedCallback* cb);
    // ScoreReceivedCallback* getScoreReceivedCallback();
};
