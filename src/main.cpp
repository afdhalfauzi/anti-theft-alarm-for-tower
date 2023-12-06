#include "Arduino.h"
#include "Radar.h"
#include "Telebot.h"
#include "Accelero.h"
#include <EEPROM.h>

#define RELAY_PIN 23
#define EEPROM_ADDRESS 0
#define EEPROM_SIZE 8

Radar radars;
Telebot telebot;
Accelero accelero;

int8_t bodysign;
bool isVibrate;
float vibrationThreshold_g;

void radarTask(void *parameter);
void acceleroTask(void *parameter);

void setup()
{
  Serial.begin(115200);
  // telebot.begin();
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);

  EEPROM.begin(EEPROM_SIZE);
  vibrationThreshold_g = EEPROM.readFloat(EEPROM_ADDRESS);
  Serial.print("Current Treshold: ");
  Serial.println(vibrationThreshold_g);
  Serial.println("Enter new threshold:");

  xTaskCreate(acceleroTask, "accelero task", 1024 * 5, NULL, 1, NULL);
  xTaskCreate(radarTask, "radar task", 1024 * 5, NULL, 2, NULL);

  while (!Serial)
    ;
}

void loop()
{
  if (bodysign >= 1 && isVibrate)
  {
    digitalWrite(RELAY_PIN, LOW);
    Serial.println("ALARM ON");
    // telebot.sendMessage("");
    delay(3000);
  }
  else
    digitalWrite(RELAY_PIN, HIGH);

  Serial.printf("Bodysign:%i accX:%.2f accY:%.2f accZ:%.2f vibrThreshold:%.2f\n", bodysign, accelero.accX, accelero.accY, accelero.accZ, vibrationThreshold_g);
  if (Serial.available())
  {
    vibrationThreshold_g = Serial.readStringUntil('\n').toFloat();
    Serial.printf("Changing treshold from %.2f g to %.2f g ...\n", EEPROM.readFloat(0), vibrationThreshold_g);
    EEPROM.writeFloat(0, vibrationThreshold_g);
    EEPROM.commit();
    Serial.printf("Threshold updated, vibration will be detected if it reaches %.2f g", vibrationThreshold_g);
  }
  vTaskDelay(200);
}

void radarTask(void *parameter)
{
  while (1)
  {
    bodysign = radars.getBodysign();
    vTaskDelay(200);
  }
}

void acceleroTask(void *parameter)
{
  accelero.init();
  while (1)
  {
    accelero.update();
    isVibrate = accelero.detectVibration(vibrationThreshold_g);
  }
}