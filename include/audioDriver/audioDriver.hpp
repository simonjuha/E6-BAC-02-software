#pragma once
#include "Arduino.h"
#include "sdCardDriver/sample.hpp"
#include "sdCardDriver/sdCardDriver.hpp"
#include "audioDriver/playbackAlgorithm.hpp"
#include "booleanDriver/gateDriver.hpp"
#include <SPI.h>
#include <vector>

#include <RTOS.h>

#define AUDIO_SPI_CLK 4
#define AUDIO_SPI_DAT 5
#define AUDIO_SPI_BUS 1

#define AUDIO_SPI_LR_1 6
#define AUDIO_SPI_LR_2 7

#define AUDIO_SPI_ANDLOGIC_CS_1  16
#define AUDIO_SPI_ANDLOGIC_CS_2  17

#define NUM_OF_CHANNELS 4

#define DEFAULT_SAMPLERATE 44100
#define SAMPLE_INTERVAL 1000000/DEFAULT_SAMPLERATE // in microseconds

enum GateMode{
    HOLD_MODE,
    TRIGGER_ONCE_MODE,
    LOOP_MODE
};

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
        pinMode(AUDIO_SPI_LR_1, OUTPUT);
        pinMode(AUDIO_SPI_LR_2, OUTPUT);
        pinMode(AUDIO_SPI_ANDLOGIC_CS_1, OUTPUT);
        pinMode(AUDIO_SPI_ANDLOGIC_CS_2, OUTPUT);
        for(int i = 0; i < SdCardDriver::getInstance().getNumberOfSamples(); i++){
            _sampleMap[i] = SdCardDriver::getInstance().getSampleNames()[i];
        }
        algorithms[0] = new forwardAlgorithm();
        algorithms[1] = new forwardAlgorithm();
        algorithms[2] = new forwardAlgorithm();
        algorithms[3] = new forwardAlgorithm();

        // load default samples (temporay fix)
        SdCardDriver::getInstance().getSampleByName("tone.wav", sampleSlot[0]);
        SdCardDriver::getInstance().getSampleByName("tone.wav", sampleSlot[1]);
        SdCardDriver::getInstance().getSampleByName("tone.wav", sampleSlot[2]);
        SdCardDriver::getInstance().getSampleByName("tone.wav", sampleSlot[3]);

        // load default volume
        _sampleVolume[0] = 0.5f;
        _sampleVolume[1] = 0.5f;
        _sampleVolume[2] = 0.5f;
        _sampleVolume[3] = 0.5f;


    }

    void tick(){
        static uint32_t lastSampleTime = 0;
        uint32_t currentTime = micros();
        _gateInputs.tick();
        if(currentTime - lastSampleTime >= sampleInterval){

            /* CHANNEL 0 SELECT */
            digitalWrite(AUDIO_SPI_ANDLOGIC_CS_1, HIGH);
            digitalWrite(AUDIO_SPI_LR_1, HIGH);

            if(gate(0))
            {
                int8_t sampleByte0 = (int8_t)algorithms[0]->play(sampleSlot[0]);
                vspi->transfer(static_cast<uint8_t>(sampleByte0* _sampleVolume[0])) ;
            }

            /* CHANNEL 1 SELECT */
            digitalWrite(AUDIO_SPI_LR_1, LOW);
            if(gate(1))
            {
                int8_t sampleByte1 = (int8_t)algorithms[1]->play(sampleSlot[1]);
                vspi->transfer(static_cast<uint8_t>(sampleByte1* _sampleVolume[1]));
            }

            /* CHANNEL 2 SELECT */
            digitalWrite(AUDIO_SPI_ANDLOGIC_CS_1, LOW);
            digitalWrite(AUDIO_SPI_ANDLOGIC_CS_2, HIGH);
            digitalWrite(AUDIO_SPI_LR_2, HIGH);

            if(gate(2))
            {
                int8_t sampleByte2 = (int8_t)algorithms[2]->play(sampleSlot[2]);
                vspi->transfer(static_cast<uint8_t>(sampleByte2* _sampleVolume[2])) ;
            }

            /* CHANNEL 3 SELECT */
            digitalWrite(AUDIO_SPI_LR_2, LOW);

            if(gate(3)){
            int8_t sampleByte3 = (int8_t)algorithms[3]->play(sampleSlot[3]);
            vspi->transfer(static_cast<uint8_t>(sampleByte3* _sampleVolume[3]));
            }

            digitalWrite(AUDIO_SPI_ANDLOGIC_CS_2, LOW);
            
            lastSampleTime = currentTime;
        }
    }
    void selectChannel(int channel){
        if(channel == 0){
            ESP_LOGI("AudioDriver", "Global channel selected");
            return;
        }
        int newChannel = channel-1;
        if(newChannel >= 0 && newChannel < NUM_OF_CHANNELS){
            _channelSelect = newChannel;
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
            else if(newValue == 3){
                algorithms[_channelSelect] = new granularAlgorithm();
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
        if(name == "gateMode"){
            if(newValue == 0){
                _gateModes[_channelSelect] = HOLD_MODE;
            }else if(newValue == 1){
                _gateModes[_channelSelect] = TRIGGER_ONCE_MODE;
            }else if(newValue == 2){
                _gateModes[_channelSelect] = LOOP_MODE;
            }else{
                ESP_LOGE("AudioDriver", "Invalid gate mode value");
            }
            return;
        }
    }

    private:
    SPIClass *vspi;
    int _channelSelect = 0;
    Sample sampleSlot[NUM_OF_CHANNELS]; // 4 samples to be played
    playbackAlgorithm * algorithms[NUM_OF_CHANNELS];
    double _sampleVolume[NUM_OF_CHANNELS];
    GateMode _gateModes[NUM_OF_CHANNELS] = {HOLD_MODE, HOLD_MODE, HOLD_MODE, HOLD_MODE};
    bool triggeredSamples[NUM_OF_CHANNELS] = {false, false, false, false};
    GateInputs _gateInputs;
    uint32_t sampleInterval = SAMPLE_INTERVAL;
    std::map<int, std::string> _sampleMap;
    // singleton stuff
    AudioDriver(){};
    AudioDriver(AudioDriver const&); // copy constructor is private
    AudioDriver& operator=(AudioDriver const&); // assignment operator is private
    ~AudioDriver(){};
    // method decides if something is transfered or not
    bool gate(int gateChannel){
        GateMode mode = _gateModes[gateChannel];
        if(gateChannel >= 0 && gateChannel < NUM_OF_CHANNELS){
            bool rise = _gateInputs.risingEdge(gateChannel);
            bool gateHigh = _gateInputs.read(gateChannel);
            bool sampleEnd = algorithms[gateChannel]->isAtEnd(sampleSlot[gateChannel]);
            switch (mode) {
                case HOLD_MODE:{
                    // gate HIGH: keep transfering
                    if(gateHigh == true){
                        return true;
                    }

                    // gate LOW: dont transfer
                    if(gateHigh == false){
                        vspi->transfer(0);
                        return false;
                    }
                    break;
                }

                case TRIGGER_ONCE_MODE: {
                    // trigger
                    if(rise){
                        triggeredSamples[gateChannel] = true;
                        return true;
                    }

                    // when sample is playing
                    if(triggeredSamples[gateChannel] == true){
                        // keep playing if sample is not at end
                        if(sampleEnd == false){
                            return true;
                        }
                        // stop playing if sample is at end
                        if(sampleEnd == true){
                            triggeredSamples[gateChannel] = false;
                            vspi->transfer(0);
                            return false;
                        }
                    }

                    vspi->transfer(0);
                    return false;
                }

                case LOOP_MODE: {
                    return true; // just loop
                    break;
                }
                default: {
                    ESP_LOGE("AudioDriver", "Invalid gate mode");
                    return false;
                    break;
                }
            }
        }
        
        ESP_LOGE("AudioDriver", "Invalid gate channel");
        return false; 
    }
};