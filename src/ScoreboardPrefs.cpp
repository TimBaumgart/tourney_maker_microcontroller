#include "ScoreboardPrefs.h"

Preferences ScoreboardPrefs::prefs; // correct static member definition

void ScoreboardPrefs::begin()
{
    prefs.begin(NAMESPACE, false);
}

std::string ScoreboardPrefs::getGlobalFieldId()
{
    String value = prefs.getString(KEY_GLOBAL_FIELD_ID, "no-global-field-id-set");
    return value.c_str();
}

void ScoreboardPrefs::setGlobalFieldId(const std::string &id)
{
    Serial.println(("writing id " + id + "...").c_str());
    prefs.putString(KEY_GLOBAL_FIELD_ID, id.c_str());
}

void ScoreboardPrefs::setScore(uint8_t score1, uint8_t score2)
{
    prefs.putInt(KEY_SCORE1, score1);
    prefs.putInt(KEY_SCORE2, score2);
}

uint8_t ScoreboardPrefs::getScore1Or0()
{
    return prefs.getInt(KEY_SCORE1, 0);
}

uint8_t ScoreboardPrefs::getScore2Or0()
{
    return prefs.getInt(KEY_SCORE2, 0);
}
