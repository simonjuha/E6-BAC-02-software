#include "Arduino.h"
#include "sdCardDriver/sample.hpp"
#include <SPI.h>
#include <vector>

#define AUDIO_SPI_CLK 4
#define AUDIO_SPI_DAT 5
#define AUDIO_SPI_LR  6
#define AUDIO_SPI_BUS 1

#define NUM_OF_SAMPLES 2

// this class needs to be optimized for speed as much as possible
// BU9480F
class AudioDriver {
    public:
    AudioDriver(){
        vspi = new SPIClass(AUDIO_SPI_BUS);
        vspi->setFrequency(10000000);
        vspi->begin(AUDIO_SPI_CLK, 7, AUDIO_SPI_DAT);
        pinMode(AUDIO_SPI_LR, OUTPUT);
    }

    void setSampleSlot(int slot, Sample * sample){
        this->sampleSlot[slot] = sample;
        this->currentPosition[slot] = 0;
    }

    // doesn't work in loop() ?
    void tick(){
        digitalWrite(AUDIO_SPI_LR, HIGH);
        vspi->transfer(sampleSlot[0]->buffer[currentPosition[0]]);
        digitalWrite(AUDIO_SPI_LR, LOW);
        vspi->transfer(sampleSlot[1]->buffer[currentPosition[1]]);
        
        currentPosition[0]++;
        if(currentPosition[0] > this->sampleSlot[0]->size){
            this->currentPosition[0] = 0;
        }
        currentPosition[1]++;
        if(currentPosition[1] > this->sampleSlot[1]->size){
            this->currentPosition[1] = 0;
        }
    }

    private:
    SPIClass *vspi;
    Sample * sampleSlot[NUM_OF_SAMPLES]; // 4 samples to be played
    int currentPosition[NUM_OF_SAMPLES]; // current position in the sample

};