#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include "sdCardDriver/SDcardInterfaces.hpp"

#define SD_SPI_CS     10  // chip select
#define SD_SPI_MOSI   11  // mosi
#define SD_SPI_MISO   12  // miso
#define SD_SPI_CLK    13  // sck

#define SD_SPI_BUS    0   // spi bus

class SdCardDriver : public ISdCardLoad, public ISdCardInfo{
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

    int getNumberOfSamples(){
        int numberOfFiles = 0;
        File root = SD.open("/");
        while(true){
            File entry =  root.openNextFile();
            if(!entry){
                break;
            }
            numberOfFiles++; // + 1 to number of files
        }
        return numberOfFiles;
    }

    std::vector<std::string> getSampleNames(){
        std::vector<std::string> fileNames;
        File root = SD.open("/");
        while(true){
            File entry =  root.openNextFile();
            if(!entry){
                break;
            }
            fileNames.push_back(entry.name()); // Add file name to vector
        }
        return fileNames;
    }

    int getSampleByName(std::string fileName, Sample *sample){
        File file = SD.open(("/"+fileName).c_str());
        static uint8_t headSize = 44;
        if(!file){
            ESP_LOGE("SD", "File not found");
            return -1;
        }
        ESP_LOGI("SD", "File found");
        // write data to sample buffer
        sample->size = (file.size() - headSize) / sizeof(int16_t); // subtract header size and divide by 2 (16 bit)
        delete[] sample->buffer; // deallocate old buffer
        sample->buffer = new int16_t[sample->size];

        // skip first 44 bytes (header)
        file.seek(44);

        // read data
        for(int i = 0; i < sample->size; i++){
            uint8_t lsb = file.read();
            uint8_t msb = file.read();
            sample->buffer[i] = static_cast<int16_t>((msb << 8) | lsb); // combine msb and lsb into 16 bit byte
        }

        file.close();

        return 0;
    }

    private:
    SPIClass *_hspi;
};