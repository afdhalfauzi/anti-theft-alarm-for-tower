#include <CTBot.h>

#define TELEGRAM_BOT_TOKEN "6616152032:AAEE1iPlAjfQDbT6QhmCKc1Da9ZKmC1_0hg"
#define TELEGRAM_CHAT_ID 747199885 //-4002640119

CTBot bot;

class Telebot
{
private:
    const char *ssid = "eFisheryFS";
    const char *pass = "123123123";

public:
    Telebot();
    bool begin();
    void sendMessage(char *msg);
};

Telebot::Telebot()
{
}

bool Telebot::begin()
{
    Serial.println("Connecting to WiFi..");
    bot.wifiConnect(ssid, pass);
    while (!bot.testConnection())
    {
        bot.setTelegramToken(TELEGRAM_BOT_TOKEN);
        delay(500);
    }
    Serial.println("Connected");
    return true;
}

void Telebot::sendMessage(char *msg)
{
    bot.sendMessage(TELEGRAM_CHAT_ID, msg);
}
