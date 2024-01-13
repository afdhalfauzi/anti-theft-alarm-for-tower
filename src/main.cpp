#include "Arduino.h"
#include "Sensor_MPU6050.h"
#include "Sensor_mmWave.h"
#include "Bot_Telegram.h"
#include "Storage_LittleFS.h"
#include <WiFi.h>

#define RELAY_PIN 23
#define ALERT_DURATION 120000

Sensor_mmWave radar;
Sensor_MPU6050 mpu;
Bot_Telegram telebot;
Storage_LittleFS config;

bool isHumanDetected;
bool isVibrate;

void eventDetection(void *parameter);
void radarTask(void *parameter);
void acceleroTask(void *parameter);
void scheduledMessage(void *parameter);
void updatesViaSerial(void *parameter);

void setup()
{
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);

  xTaskCreate(acceleroTask, "mpu task", 1024 * 10, NULL, 1, NULL);
  xTaskCreate(eventDetection, "detect event", 1024 * 10, NULL, 2, NULL);
  xTaskCreate(updatesViaSerial, "threshold updates", 1024 * 10, NULL, 3, NULL);
  xTaskCreate(radarTask, "radar task", 1024 * 5, NULL, 4, NULL);
  xTaskCreate(scheduledMessage, "hourly message", 1024 * 5, NULL, 5, NULL);
}

void loop()
{
  vTaskDelete(NULL);
}

void eventDetection(void *parameter)
{

  while (1)
  {
    if (isHumanDetected && isVibrate)
    {
      digitalWrite(RELAY_PIN, LOW);
      Serial.println("ALARM ON");
      telebot.sendMessage("NIU NIU");
      delay(ALERT_DURATION);
    }
    else
      digitalWrite(RELAY_PIN, HIGH);
    vTaskDelay(50);
  }
}

void radarTask(void *parameter)
{
  while (1)
  {
    isHumanDetected = (radar.getBodysign() >= 1);
    vTaskDelay(50);
  }
}

void acceleroTask(void *parameter)
{
  mpu.init();
  while (1)
  {
    isVibrate = (mpu.getAverageReading() >= config.vibrThreshold_g);
    vTaskDelay(50);
  }
}

void scheduledMessage(void *parameter)
{
  unsigned long lastMessageTime = 0;
  telebot.init();
  while (1)
  {
    if (millis() - lastMessageTime >= config.msgInterval_ms && WiFi.status() == WL_CONNECTED)
    {
      char payload[60];
      sprintf(payload, "Tilt angle: %.2f°\n", mpu.gyX);
      lastMessageTime = millis();
      telebot.sendMessage(payload);
    }
    vTaskDelay(50);
  }
}

void updatesViaSerial(void *parameter)
{
  unsigned long lastPrintTime = 0;
  config.init();
  config.load();
  while (1)
  {
    if (Serial.available())
    {
      StaticJsonDocument<200> doc;
      String jsonString = Serial.readStringUntil('\n');

      deserializeJson(doc, jsonString);

      config.vibrThreshold_g = doc["vibrThreshold"];
      config.vibrDuration_ms = doc["vibrDuration"];
      config.msgInterval_ms = doc["msgInterval"];

      config.save();

      delay(2000);
    }
    if (millis() - lastPrintTime >= 1000)
    {
      lastPrintTime = millis();
      Serial.printf("averageAcceleration:%.2fg | gyX:%.2f gyY:%.2f gyZ:%.2f\n", mpu.getAverageReading(), mpu.gyX, mpu.gyY, mpu.gyZ);
      Serial.printf("vibrThreshold:%.2fg vibrDuration:%ims msgInterval:%ims | Human:%i\n\n", config.vibrThreshold_g, config.vibrDuration_ms, config.msgInterval_ms, isHumanDetected);
    }
    vTaskDelay(50);
  }
}