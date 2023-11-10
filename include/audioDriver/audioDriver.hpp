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

        // load default volume
        _sampleVolume[0] = 0.5f;
        _sampleVolume[1] = 0.5f;

    }

    void tick(){
        static uint32_t lastSampleTime = 0;
        uint32_t currentTime = micros();
        if(currentTime - lastSampleTime >= sampleInterval){

            digitalWrite(AUDIO_SPI_LR, HIGH);
            int8_t sampleByte0 = (int8_t)algorithms[0]->play(sampleSlot[0]);
            vspi->transfer(static_cast<uint8_t>(sampleByte0* _sampleVolume[0])) ;

            digitalWrite(AUDIO_SPI_LR, LOW);
            int8_t sampleByte1 = (int8_t)algorithms[1]->play(sampleSlot[1]);
            vspi->transfer(static_cast<uint8_t>(sampleByte1* _sampleVolume[1]));
            
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
        if(name == "playback"){
            if (algorithms[_channelSelect] != nullptr){
                delete algorithms[_channelSelect];
            }
            if(newValue == 0){
                algorithms[_channelSelect] = new forwardAlgorithm();
            }else if(newValue == 1){
                algorithms[_channelSelect] = new backwardAlgorithm();
            }
            else if(newValue == 2){
                algorithms[_channelSelect] = new pingPongAlgorithm();
            }
            else{
                ESP_LOGW("AudioDriver", "Invalid algorithm value");
            }
            return;
        }
        if(name == "sampleRate"){
            if(newValue > 0 && newValue < DEFAULT_SAMPLERATE*2){
                sampleInterval = 1000000/(int)newValue;
            }
            return;
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
            return;
        }
        if(name == "volume"){
            if(newValue < 0){
                _sampleVolume[_channelSelect] = 0;
                return;
            }
            if(newValue > 1){
                _sampleVolume[_channelSelect] = 1;
                return;
            }
            _sampleVolume[_channelSelect] = newValue;
            return;
        }
    }

    private:
    SPIClass *vspi;
    int _channelSelect = 0;
    Sample sampleSlot[NUM_OF_CHANNELS]; // 4 samples to be played
    playbackAlgorithm * algorithms[NUM_OF_CHANNELS];
    double _sampleVolume[NUM_OF_CHANNELS];
    uint32_t sampleInterval = SAMPLE_INTERVAL;
    std::map<int, std::string> _sampleMap;
    // singleton stuff
    AudioDriver(){};
    AudioDriver(AudioDriver const&); // copy constructor is private
    AudioDriver& operator=(AudioDriver const&); // assignment operator is private
    ~AudioDriver(){};
};