#include <Arduino.h>
#include <SPI.h>
#include <SD.h>

#define SD_SPI_CS     10  // chip select
#define SD_SPI_MOSI   11  // mosi
#define SD_SPI_MISO   12  // miso
#define SD_SPI_CLK    13  // sck

#define SD_SPI_BUS    0   // spi bus

class SdCardDriver{
    public:
    SdCardDriver(){
        _hspi = new SPIClass(SD_SPI_BUS);
        _hspi->begin(SD_SPI_CLK, SD_SPI_MISO, SD_SPI_MOSI, SD_SPI_CS);
        pinMode(SD_SPI_CS, OUTPUT);
        pinMode(SD_SPI_MISO, INPUT);
        if (!SD.begin(SD_SPI_CS, *_hspi)) {
            ESP_LOGE("SD", "Card Mount Failed");
        }else{
            ESP_LOGI("SD", "Card Mount Success");
        }
    }
    private:
    SPIClass *_hspi;
};