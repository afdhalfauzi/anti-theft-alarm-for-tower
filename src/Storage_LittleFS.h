#pragma once

#include <LittleFS.h>
#include <ArduinoJson.h>
#define FORMAT_LITTLEFS_IF_FAILED true

// define filename to save config file

class Storage_LittleFS
{
private:
    const String config_filename = "/settings.json";
    bool writeFile(String filename, String dataJson);
    String readFile(String filename);

public:
    bool init();
    bool load();
    bool save();
    float vibrThreshold_g;
    int vibrDuration_ms;
    int msgInterval_ms;
};