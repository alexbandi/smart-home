#include <Arduino.h>
#include <ArduinoLog.h>
#include "sensors/Sht35.h"

void setup()
{
    Serial.begin(115200);

    Log.begin(LOG_LEVEL_VERBOSE, &Serial);

    if (!Sht35::init())
    {
        Log.fatalln("Could not find the SHT35 sensor. Make sure it has been connected properly.");
        while (true)
            delay(10);
    }
}

void loop()
{
    float temp;
    float humidity;
    if (Sht35::getBoth(&temp, &humidity)) {
        Log.infoln("Temperature: %.2f °C", temp);
        Log.infoln("Humidity: %.2f %", humidity);
    } else {
        Log.errorln("Could not read temperature/humidity.");
    }

    delay(1000);
}

