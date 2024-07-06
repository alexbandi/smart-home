#include "Wire.h"
#include <SPI.h> // Needed for "Adafruit_SHT31.h"
#include "Adafruit_SHT31.h"

// SHT35 interface
#define SHT35_ADDRESS 0x44 // Default I2C address of the SHT35
// SHT35 interface
Adafruit_SHT31 sht31 = Adafruit_SHT31();

void setup()
{
    Serial.begin(115200);
    while (!Serial)
        delay(10); // Wait for serial monitor

    if (!sht31.begin(SHT35_ADDRESS))
    {
        Serial.println("Could not find the SHT35 sensor. Make sure it has been connected properly.");
        while (true)
            delay(10);
    }
}

void loop()
{
    float temp;
    float humidity;
    if (sht31.readBoth(&temp, &humidity)) {

        Serial.print("Temperature: ");
        Serial.print(temp);
        Serial.println(" °C");

                Serial.print("Luftfeuchtigkeit: ");
        Serial.print(humidity);
        Serial.println(" %");
    } else {
        Serial.println("Could not read temperature/humidity.");
    }

    delay(1000);
}

