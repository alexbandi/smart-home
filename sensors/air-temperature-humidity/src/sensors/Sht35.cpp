#include "Sht35.h"
#include <SPI.h> // Needed for Adafruit_SHT31.h
#include <Adafruit_SHT31.h>

// SHT35 interface
#define SHT35_ADDRESS 0x44 // Default I2C address of the SHT35
// SHT35 interface
Adafruit_SHT31 sht31 = Adafruit_SHT31();

bool Sht35::init() {
    return sht31.begin(SHT31_DEFAULT_ADDR);
}

float Sht35::getHumidity() {
    return sht31.readTemperature();
}

float Sht35::getTemperature() {
    return sht31.readHumidity();
}

bool Sht35::getBoth(float *temperature, float *humidity) {
    return sht31.readBoth(temperature, humidity);
}

