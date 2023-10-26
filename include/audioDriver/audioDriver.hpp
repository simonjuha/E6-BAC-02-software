#include "Arduino.h"
#include "sdCardDriver/sample.hpp"
#include "audioDriver/playbackAlgorithm.hpp"
#include <SPI.h>
#include <vector>

#include <RTOS.h>

#define AUDIO_SPI_CLK 4
#define AUDIO_SPI_DAT 5
#define AUDIO_SPI_LR  6
#define AUDIO_SPI_BUS 1

#define NUM_OF_SAMPLES 2

#define SAMPLERATE 44100
#define SAMPLE_INTERVAL 1000000/SAMPLERATE // in microseconds

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
        this->algorithms[slot] = new forwardAlgorithm();
    }

    // doesn't work in loop() ?
    void tick(){
        static uint32_t lastSampleTime = 0;
        uint32_t currentTime = micros();

        if(currentTime - lastSampleTime >= SAMPLE_INTERVAL){
            digitalWrite(AUDIO_SPI_LR, HIGH);
            algorithms[0]->play(sampleSlot[0], vspi);
            digitalWrite(AUDIO_SPI_LR, LOW);
            algorithms[1]->play(sampleSlot[1], vspi);
            lastSampleTime = currentTime;
        }
    }

    private:
    SPIClass *vspi;
    Sample * sampleSlot[NUM_OF_SAMPLES]; // 4 samples to be played
    int currentPosition[NUM_OF_SAMPLES]; // current position in the sample
    playbackAlgorithm * algorithms[NUM_OF_SAMPLES];

};