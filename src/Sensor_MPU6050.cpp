#include "Sensor_MPU6050.h"

bool Sensor_MPU6050::init()
{
    Wire.begin();

    byte status = _mpu.begin();
    Serial.print(F("MPU6050 status: "));
    Serial.println(status);
    while (status != 0)
    {
    } // stop everything if could not connect to MPU6050

    Serial.println(F("Calculating offsets, do not move MPU6050"));
    delay(1000);
    _mpu.calcOffsets();
    Serial.println("Done!\n");

    return 1;
}

void Sensor_MPU6050::update()
{
    _mpu.update();
    accX = _mpu.getAccX();
    accY = _mpu.getAccY();
    accZ = _mpu.getAccZ() - 1;
    gyX = _mpu.getAngleX();
    gyY = _mpu.getAngleY();
    gyZ = _mpu.getAngleZ();
}

void Sensor_MPU6050::printValue()
{
    update();
    Serial.print("X:");
    Serial.print(accX);
    Serial.print(" Y:");
    Serial.print(accY);
    Serial.print(" Z:");
    Serial.print(accZ);
    Serial.print(" X:");
    Serial.print(gyX);
    Serial.print(" Y:");
    Serial.print(gyY);
    Serial.print(" Z:");
    Serial.println(gyZ);
}

float Sensor_MPU6050::getTotalAcceleration()
{
    return abs(accX) + abs(accY) + abs(accZ);
}

float Sensor_MPU6050::getAverageReading()
{
    update();
    readings[index] = getTotalAcceleration();
    for (int i = 0; i < NUM_OF_READINGS; i++)
    {
        average += readings[i];
    }
    average /= NUM_OF_READINGS;
    index = (index + 1) % NUM_OF_READINGS;

    return average;
}

bool Sensor_MPU6050::detectVibration(float treshold)
{
    update();
    // if (abs(accX) >= treshold || abs(accY) >= treshold || abs(accZ) >= treshold)
    if (abs(accX) + abs(accY) + abs(accZ) >= treshold)
        return 1;
    return 0;
}