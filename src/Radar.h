#include <humanstaticLite.h>

class Radar
{
private:
    HumanStaticLite radar = HumanStaticLite(&Serial2);

public:
    Radar();
    int8_t getBodysign();
};

Radar::Radar()
{
    Serial2.begin(115200);
}

int8_t Radar::getBodysign()
{
    // wait till data is valid then return
    do
    {
        radar.HumanStatic_func(true); // Turn on printing of human movement sign parameters
        delay(200);
    } while (radar.radarStatus == 0x00);
    return radar.bodysign_val;
}
