#pragma once
#include <Arduino.h>
#include <Preferences.h>
#include <string>

class ScoreboardPrefs
{
public:
    static void begin();
    static std::string getGlobalFieldId();
    static void setGlobalFieldId(const std::string &id);
    static void setScore(uint8_t score1, uint8_t score2);
    static uint8_t getScore1Or0();
    static uint8_t getScore2Or0();

private:
    static Preferences prefs; // <- static member
    static constexpr const char *NAMESPACE = "tourney-maker";
    static constexpr const char *KEY_GLOBAL_FIELD_ID = "GLOBAL_FIELD_ID";
    static constexpr const char *KEY_SCORE1 = "SCORE1";
    static constexpr const char *KEY_SCORE2 = "SCORE2";
};
