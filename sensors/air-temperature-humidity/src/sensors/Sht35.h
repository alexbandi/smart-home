#ifndef SHT35_H
#define SHT35_H

class Sht35
{
public:
    static bool init();
    static float getTemperature();
    static float getHumidity();
    static bool getBoth(float *temperature, float *humidity);
};

#endif // SHT35_H