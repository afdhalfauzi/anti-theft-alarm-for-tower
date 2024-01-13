#include "Storage_LittleFS.h"

bool Storage_LittleFS::init()
{
    // Mount LITTLEFS and read in config file
    if (!LittleFS.begin(false))
    {
        Serial.println("LITTLEFS Mount failed");
        Serial.println("Did not find filesystem; starting format");
        // format if begin fails
        if (!LittleFS.begin(true))
        {
            Serial.println("LITTLEFS mount failed");
            Serial.println("Formatting not possible");
            return false;
        }
        else
        {
            Serial.println("Formatting");
        }
    }
    else
    {
        Serial.println("setup -> SPIFFS mounted successfully");
        if (load() == false)
        {
            Serial.println("setup -> Could not read Config file -> initializing new file");
            // if not possible -> save new config file
            if (save())
            {
                Serial.println("setup -> Config file saved");
            }
        }

        // Serial.println("Value1 = " + String(value1) + ", value2 = " + String(value2) + ", user_string = " + user_string);
    }
    return true;
}

bool Storage_LittleFS::writeFile(String filename, String dataJson)
{
    File file = LittleFS.open(filename, "w");
    if (!file)
    {
        Serial.println("writeFile -> failed to open file for writing");
        return false;
    }
    if (file.print(dataJson))
    {
        Serial.println("File written");
    }
    else
    {
        Serial.println("Write failed");
    }
    file.close();
    return true;
}

String Storage_LittleFS::readFile(String filename)
{
    File file = LittleFS.open(filename);
    if (!file)
    {
        Serial.println("Failed to open file for reading");
        return "";
    }

    String fileText = "";
    while (file.available())
    {
        fileText = file.readString();
    }
    file.close();
    return fileText;
}

bool Storage_LittleFS::load()
{
    String file_content = readFile(config_filename);

    int config_file_size = file_content.length();
    Serial.println("Config file size: " + String(config_file_size));

    if (config_file_size > 200)
    {
        Serial.println("Config file too large");
        return false;
    }
    StaticJsonDocument<200> doc;
    auto error = deserializeJson(doc, file_content);
    if (error)
    {
        Serial.println("Error interpreting config file");
        return false;
    }

    const float _value1 = doc["vibrThreshold"];
    const int _value2 = doc["vibrDuration"];
    const int _value3 = doc["msgInterval"];

    vibrThreshold_g = _value1;
    vibrDuration_ms = _value2;
    msgInterval_ms = _value3;
    return true;
}

bool Storage_LittleFS::save()
{
    StaticJsonDocument<200> doc;

    // write variables to JSON file
    doc["vibrThreshold"] = vibrThreshold_g;
    doc["vibrDuration"] = vibrDuration_ms;
    doc["msgInterval"] = msgInterval_ms;

    // write config file
    String tmp = "";
    serializeJson(doc, tmp);
    writeFile(config_filename, tmp);

    return true;
}