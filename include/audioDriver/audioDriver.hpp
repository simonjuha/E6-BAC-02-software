#pragma once
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

#define DEFAULT_SAMPLERATE 44100
#define SAMPLE_INTERVAL 1000000/DEFAULT_SAMPLERATE // in microseconds

// this class needs to be optimized for speed as much as possible
// BU9480F
class AudioDriver : public IParameterObserver{
    public:
    static AudioDriver& getInstance(){
        static AudioDriver instance;
        return instance;
    }
    void init(){
        vspi = new SPIClass(AUDIO_SPI_BUS);
        vspi->setFrequency(10000000);
        vspi->begin(AUDIO_SPI_CLK, 7, AUDIO_SPI_DAT);
        pinMode(AUDIO_SPI_LR, OUTPUT);
    }

    void setSampleSlot(int slot, Sample * sample){
        this->sampleSlot[slot] = sample;
        this->algorithms[slot] = new forwardAlgorithm();
    }

    // doesn't work in loop() ?
    void tick(){
        static uint32_t lastSampleTime = 0;
        uint32_t currentTime = micros();
        if(sampleSlot[0] == nullptr || sampleSlot[1] == nullptr){
            ESP_LOGE("AudioDriver", "No sample loaded");
            return;
        }
        if(currentTime - lastSampleTime >= sampleInterval){
            digitalWrite(AUDIO_SPI_LR, HIGH);
            algorithms[0]->play(sampleSlot[0], vspi);
            digitalWrite(AUDIO_SPI_LR, LOW);
            algorithms[1]->play(sampleSlot[1], vspi);
            lastSampleTime = currentTime;
        }
    }
    void selectChannel(int channel){
        _channelSelect = (channel >= 0 && channel <= 1 ) ? channel : 0;
    }

    // maybe add a class "modifyer" to pass value to
    void update(const std::string& name, float newValue) override{
        if(name == "algorithm"){
            delete algorithms[_channelSelect];
            if(newValue == 0){
                algorithms[0] = new forwardAlgorithm();
            }else if(newValue == 1){
                algorithms[0] = new backwardAlgorithm();
            }
            else{
                ESP_LOGW("AudioDriver", "Invalid algorithm value");
            }
        }
        if(name == "sampleRate"){
            if(newValue > 0 && newValue < DEFAULT_SAMPLERATE*2){
                sampleInterval = 1000000/(int)newValue;
            }
        }
    }

    private:
    SPIClass *vspi;
    Sample * sampleSlot[NUM_OF_SAMPLES]; // 4 samples to be played
    playbackAlgorithm * algorithms[NUM_OF_SAMPLES];
    uint32_t sampleInterval = SAMPLE_INTERVAL;
    int _channelSelect = 0;
    // singleton stuff
    AudioDriver(){};
    AudioDriver(AudioDriver const&); // copy constructor is private
    AudioDriver& operator=(AudioDriver const&); // assignment operator is private
    ~AudioDriver(){};
};