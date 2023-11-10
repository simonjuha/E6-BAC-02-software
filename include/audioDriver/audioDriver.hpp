#pragma once
#include "Arduino.h"
#include "sdCardDriver/sample.hpp"
#include "sdCardDriver/sdCardDriver.hpp"
#include "audioDriver/playbackAlgorithm.hpp"
#include <SPI.h>
#include <vector>

#include <RTOS.h>

#define AUDIO_SPI_CLK 4
#define AUDIO_SPI_DAT 5
#define AUDIO_SPI_LR  6
#define AUDIO_SPI_BUS 1

#define NUM_OF_CHANNELS 2

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
        for(int i = 0; i < SdCardDriver::getInstance().getNumberOfSamples(); i++){
            _sampleMap[i] = SdCardDriver::getInstance().getSampleNames()[i];
        }
        algorithms[0] = new forwardAlgorithm();
        algorithms[1] = new forwardAlgorithm();

        // load default samples (temporay fix)
        SdCardDriver::getInstance().getSampleByName("tone.wav", sampleSlot[0]);
        SdCardDriver::getInstance().getSampleByName("tone.wav", sampleSlot[1]);

    }

    // doesn't work in loop() ?
    void tick(){
        static uint32_t lastSampleTime = 0;
        uint32_t currentTime = micros();
        if(currentTime - lastSampleTime >= sampleInterval){
            digitalWrite(AUDIO_SPI_LR, HIGH);
            algorithms[0]->play(sampleSlot[0], vspi);
            digitalWrite(AUDIO_SPI_LR, LOW);
            algorithms[1]->play(sampleSlot[1], vspi);
            lastSampleTime = currentTime;
        }
    }
    void selectChannel(int channel){
        if(channel >= 0 && channel < NUM_OF_CHANNELS){
            _channelSelect = channel;
        }else{
            ESP_LOGE("AudioDriver", "Invalid channel value");
        }
    }

    // maybe add a class "modifyer" to pass value to
    void update(const std::string& name, float newValue) override{
        if(name == "algorithm"){
            if (algorithms[_channelSelect] != nullptr){
                delete algorithms[_channelSelect];
            }
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
        if(name == "sample"){
            if(newValue >= 0 && newValue < _sampleMap.size() ){
                std::string searchName = _sampleMap[(int)newValue];
                if(SdCardDriver::getInstance().getSampleByName(searchName, sampleSlot[_channelSelect]) != 0){
                    ESP_LOGE("AudioDriver", "Sample: %s not found", _sampleMap[newValue].c_str());
                    return;
                }
            }else{
                ESP_LOGE("AudioDriver", "Invalid sample value");
            }

        }
    }

    private:
    SPIClass *vspi;
    Sample sampleSlot[NUM_OF_CHANNELS]; // 4 samples to be played
    playbackAlgorithm * algorithms[NUM_OF_CHANNELS];
    uint32_t sampleInterval = SAMPLE_INTERVAL;
    int _channelSelect = 0;
    std::map<int, std::string> _sampleMap;
    // singleton stuff
    AudioDriver(){};
    AudioDriver(AudioDriver const&); // copy constructor is private
    AudioDriver& operator=(AudioDriver const&); // assignment operator is private
    ~AudioDriver(){};
};