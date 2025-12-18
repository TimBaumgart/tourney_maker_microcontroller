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

private:
    static Preferences prefs;
    static constexpr const char *NAMESPACE = "tourney-maker";
    static constexpr const char *KEY_GLOBAL_FIELD_ID = "GLOBAL_FIELD_ID";
};