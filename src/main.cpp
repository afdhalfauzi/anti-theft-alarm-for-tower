#include "Arduino.h"
#include "Radar.h"
#include "Telebot.h"
#include "Accelero.h"

#define RELAY_PIN 23
#define PIR_PIN 22
#define VIBRATION_TRESHOLD 0.5

Radar radars;
Telebot telebot;
Accelero accelero;

int8_t bodysign;
bool isVibrate;

void radarTask(void *parameter);
void acceleroTask(void *parameter);
void setup()
{
  Serial.begin(115200);
  telebot.begin();
  pinMode(PIR_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);

  while (!Serial)
    ;
}

void loop()
{
  switch (bodysign)
  /*0     No one detected in surrounding environment
    1     environment is detected to be occupied and in a stationary state
    2-100 surroundings are detected to be occupied and active*/
  {
  case 0:
    Serial.println("No human");
    Serial.println("---------------------------------");
    break;
  case 1 ... 6:
    Serial.println("Human detected");
    Serial.println("---------------------------------");
    break;
  case 7 ... 100:
    Serial.println("Human activity detected");
    Serial.println("---------------------------------");
    break;
  }
  if (bodysign >= 1 && isVibrate)
  {
    digitalWrite(RELAY_PIN, LOW);
    telebot.sendMessage("");
  }
  else
    digitalWrite(RELAY_PIN, HIGH);
  delay(200);
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
    isVibrate = accelero.detectVibration(VIBRATION_TRESHOLD);
  }
}