// #include "Sensor_mmWave.h"

// Sensor_mmWave::Sensor_mmWave()
// {
//     Serial2.begin(115200);
// }

// int8_t Sensor_mmWave::getBodysign()
// {
//     // wait till data is valid then return
//     do
//     {
//         mmWave.HumanStatic_func(true); // Turn on printing of human movement sign parameters
//         vTaskDelay(200);
//     } while (mmWave.radarStatus == 0x00);
//     return mmWave.bodysign_val;
// }