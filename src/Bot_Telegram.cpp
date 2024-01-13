#include "Bot_Telegram.h"

bool Bot_Telegram::init()
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

void Bot_Telegram::sendMessage(char *msg)
{
    bot.sendMessage(TELEGRAM_CHAT_ID, msg);
    Serial.println(msg);
}