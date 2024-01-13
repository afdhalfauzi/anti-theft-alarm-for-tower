#pragma once

#include <CTBot.h>

#define TELEGRAM_BOT_TOKEN "6616152032:AAEE1iPlAjfQDbT6QhmCKc1Da9ZKmC1_0hg"
#define TELEGRAM_CHAT_ID 747199885 //-4002640119

class Bot_Telegram
{
private:
    CTBot bot;
    const char *ssid = "eFisheryFS";
    const char *pass = "123123123";

public:
    bool init();
    void sendMessage(char *msg);
};