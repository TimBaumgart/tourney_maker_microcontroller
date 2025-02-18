
#include <WString.h>

class TourneyMakerScoreboard {
    public:
        String name;
        uint8_t score1;
        uint8_t score2;

        TourneyMakerScoreboard(String name);

    void bumpScore();
    void setScore(uint8_t score1, uint8_t score2);
    void scoreReceived(uint8_t score1, uint8_t score2);

    static TourneyMakerScoreboard *setup(String name);
};