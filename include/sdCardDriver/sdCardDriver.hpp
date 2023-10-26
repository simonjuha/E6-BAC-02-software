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
        File file = SD.open(("/"+fileName).c_str());    // open wav file
        static uint8_t headSize = 44;                   // wav header size in bytes
        if(!file){
            ESP_LOGE("SD", "File not found");
            return -1;
        }
        ESP_LOGI("SD", "File found");
        
        // CALCULATE SAMPLE SIZE
        int sampleSize = getSampleSize(&file);                                      // get sample size in 16 bit bytes
        int zeroPadding = file.size() - headSize - (sampleSize * sizeof(int16_t));  // zero padding in 8bit bytes preceeding sample data
        sample->size = sampleSize;                                                  // subtract header size and divide by 2 (16 bit)
        delete[] sample->buffer;                                                    // deallocate old buffer
        sample->buffer = new int16_t[sample->size];                                 // allocate new buffer

        // GET WAV DATA BLOCK
        file.seek(headSize + 1 + zeroPadding);    // skip first 44 bytes (header) and skip zero padding
        for(int i = 0; i < sample->size; i++) // read data (wav file)
        {
            uint8_t lsb = file.read();
            uint8_t msb = file.read();
            sample->buffer[i] = static_cast<int16_t>((msb << 8) | lsb); // combine msb and lsb into 16 bit byte
        }
        file.close();
        return 0;
    }

    private:
    SPIClass *_hspi;
    int getSampleSize(File* file){
        // get sample_count from head (line 9)
        // skip first 8 lines (\n)
        for(int i = 0; i < 8; i++){
            file->readStringUntil('\n');
        }
        // if next bytes form "sample_count -i " then read the number
        if(file->findUntil("sample_count -i ", "\n")){
            int sample_count = file->parseInt();
            ESP_LOGV("SD", "sample_count found: %d", sample_count);
            return sample_count;
        }
        ESP_LOGE("SD", "sample_count not found");
        return -1;
    }
};