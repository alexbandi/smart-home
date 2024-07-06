#include <Arduino.h>
#include "sensors/Sht35.h"
#include "utils/Log.h"

void setup()
{
    Log::init();

    if (!Sht35::init())
    {
        Log::error("Could not find the SHT35 sensor. Make sure it has been connected properly.");
        while (true)
            delay(10);
    }
}

void loop()
{
    float temp;
    float humidity;
    if (Sht35::getBoth(&temp, &humidity)) {
        Log::info("Temperature: %.2f °C", temp);
        Log::info("Humidity: %.2f %", humidity);
    } else {
        Log::error("Could not read temperature/humidity.");
    }

    delay(1000);
}

