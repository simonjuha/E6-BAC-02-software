/*
    ESP32-S3-DevKitC-1-N8R8 (8 MB)
*/
/*
#include <Arduino.h>
#include <RTOS.h>

#include "rotaryEncoderDriver/quardratureDriver.hpp"
#include "booleanDriver/gpioDriver.hpp"
#include "displayDriver/displayDriver.hpp"
#include "cvDriver/adcDriver.hpp"
#include "ledDriver/ledDriver.hpp"
#include "sdCardDriver/sdCardDriver.hpp"
*/

#include "Arduino.h"
#include <SPI.h>
#include <SD.h>

#define SS   10
#define MOSI 11
#define MISO 12
#define SCK  13

SPIClass *hspi;

void setup() {

    Serial.begin(115200);
    Serial.print("MOSI: ");
    Serial.println(MOSI);
    Serial.print("MISO: ");
    Serial.println(MISO);
    Serial.print("SCK: ");
    Serial.println(SCK);
    Serial.print("SS: ");
    Serial.println(SS); 

    pinMode(SS, OUTPUT);
    pinMode(MISO, INPUT);

    // initialize SPI
    hspi  = new SPIClass(0);
    hspi->begin(SCK, MISO, MOSI, SS);
    
}

void loop() {

    delay(1000);
    // WORKING SPI COMMUNICATION
    /*
        hspi->beginTransaction(SPISettings(100000, MSBFIRST, SPI_MODE0));
        digitalWrite(SS, LOW);
        // send CMD0
        hspi->transfer(0x40);
        hspi->transfer(0x00);
        hspi->transfer(0x00);
        hspi->transfer(0x00);
        hspi->transfer(0x00);
        hspi->transfer(0x95);
        // receive R1
        uint8_t r1 = hspi->transfer(0xFF);
        uint8_t r2 = hspi->transfer(0xFF);
        digitalWrite(SS, HIGH);
        hspi->endTransaction();

        Serial.print("R1: ");
        Serial.println(r1, HEX);
        Serial.print("R2: ");
        Serial.println(r2, HEX);
    */

    // SD CARD COMMMUNICATION (SD.h)
    
        if (!SD.begin(SS, *hspi)) {
            Serial.println("Card Mount Failed");
            return;
        }else{
            Serial.println("Card Mount Success");
            // print file names
            File root = SD.open("/");
            File file = root.openNextFile();
            while(file){
                Serial.println(file.name());
                file = root.openNextFile();
            }
        }

}
