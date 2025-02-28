#include <Arduino.h>

class ScoreReceivedCallback {
    public:
        virtual void onScoreReceived(uint8_t score1, uint8_t score2);
};

class TourneyMakerScoreboard {
    private:
        std::string name;
        uint8_t score1;
        uint8_t score2;
        TourneyMakerScoreboard(std::string name);

    public:
        bool deviceConnected = false;
        void bumpScore();
        void setScore(uint8_t score1, uint8_t score2);
        void scoreReceived(uint8_t score1, uint8_t score2); 
        ScoreReceivedCallback* scoreReceivedCallback;
        static TourneyMakerScoreboard *setup(std::string name);
        // void setScoreReceivedCallback(ScoreReceivedCallback* cb);
        // ScoreReceivedCallback* getScoreReceivedCallback();
};
