#pragma once

#include <MPU6050_light.h>

#define NUM_OF_READINGS 30

class Sensor_MPU6050
{
private:
    MPU6050 _mpu;
    uint8_t index = 0;
    float average = 0;
    float readings[NUM_OF_READINGS];

public:
    float accX, accY, accZ;
    float gyX, gyY, gyZ;
    bool init();
    void update();
    void printValue();
    float getTotalAcceleration();
    float getAverageReading();
    bool detectVibration(float treshold);
};
