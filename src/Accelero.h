#include <MPU6050_light.h>
#include "Wire.h"

MPU6050 mpu(Wire);

class Accelero
{
private:
public:
    float accX;
    float accY;
    float accZ;
    float gyX;
    float gyY;
    float gyZ;
    Accelero();
    bool init();
    void update();
    void printValue();
    bool detectVibration(float treshold);
};

Accelero::Accelero() {}

bool Accelero::init()
{
    Wire.begin();

    byte status = mpu.begin();
    Serial.print(F("MPU6050 status: "));
    Serial.println(status);
    while (status != 0)
    {
    } // stop everything if could not connect to MPU6050

    Serial.println(F("Calculating offsets, do not move MPU6050"));
    delay(1000);
    mpu.calcOffsets(); // gyro and accelero
    Serial.println("Done!\n");

    return 1;
}

void Accelero::update()
{
    mpu.update();
    accX = mpu.getAccX();
    accY = mpu.getAccY();
    accZ = mpu.getAccZ() - 1;
    gyX = mpu.getAngleX();
    gyY = mpu.getAngleY();
    gyZ = mpu.getAngleZ();
}

void Accelero::printValue()
{
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

bool Accelero::detectVibration(float treshold)
{
    update();
    if (abs(accX) > treshold || abs(accY) > treshold || abs(accZ) > treshold)
        return 1;
    return 0;
}