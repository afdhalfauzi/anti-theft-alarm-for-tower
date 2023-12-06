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
    Accelero();
    bool init();
    void update();
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
    mpu.calcOffsets(false, true); // gyro and accelero
    Serial.println("Done!\n");

    return 1;
}

void Accelero::update()
{
    mpu.update();
    accX = mpu.getAccX();
    accY = mpu.getAccY();
    accZ = mpu.getAccZ();
}

bool Accelero::detectVibration(float treshold)
{
    update();
    if (abs(accX) > treshold || abs(accY) > treshold || abs(accZ) > treshold)
        return 1;
    return 0;
}
