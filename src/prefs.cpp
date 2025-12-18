#include "prefs.h"

Preferences ScoreboardPrefs::prefs;

void ScoreboardPrefs::begin()
{
    prefs.begin(NAMESPACE, false);
}

std::string ScoreboardPrefs::getGlobalFieldId()
{
    std::string id = prefs.getString(KEY_GLOBAL_FIELD_ID,
                                     "no-global-field-id-set")
                         .c_str();
    return id;
}

void ScoreboardPrefs::setGlobalFieldId(const std::string &id)
{

    Serial.println(("writing id " + id + "...").c_str());
    prefs.putString(KEY_GLOBAL_FIELD_ID, id.c_str());
}