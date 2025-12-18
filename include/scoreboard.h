#include <Arduino.h>
#include <ScoreCharacteristic.h>
#include <ColorCharacteristic.h>

class ScoreboardChangedCallback
{
public:
    virtual void onScoreReceived(uint8_t score1, uint8_t score2);
    virtual void onColorReceived(uint32_t color1, uint32_t color2);
};

class TourneyMakerScoreboard
{
private:
    std::string name;
    std::string globalFieldId;
    uint8_t score1;
    uint8_t score2;
    ScoreCharacteristic *scoreCharacteristic;
    ColorCharacteristic *colorCharacteristic;
    TourneyMakerScoreboard(std::string name, std::string globalFieldId);

public:
    bool deviceConnected = false;
    void connected();
    void disconnected();
    void bumpScore(uint8_t diff1, uint8_t diff2);
    void setScore(uint8_t score1, uint8_t score2);
    void scoreReceived(std::string value);
    void colorReceived(std::string value);
    ScoreboardChangedCallback *scoreboardChangedCallback;
    static TourneyMakerScoreboard *setup(std::string name);
    // void setScoreReceivedCallback(ScoreReceivedCallback* cb);
    // ScoreReceivedCallback* getScoreReceivedCallback();
};
