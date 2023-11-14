#pragma once
#include "booleanDriver/gpioDriver.hpp"
#include "Arduino.h"

// to be set
#define AUDIO_GATE_1 18
#define AUDIO_GATE_2 8
#define AUDIO_GATE_3 3
#define AUDIO_GATE_4 46

class GateInputs
{
    public:
    GateInputs() : _gates({AUDIO_GATE_1,AUDIO_GATE_2,AUDIO_GATE_3,AUDIO_GATE_4})
    {
        pinMode(AUDIO_GATE_1,INPUT_PULLDOWN);
        pinMode(AUDIO_GATE_2,INPUT_PULLDOWN);
        pinMode(AUDIO_GATE_3,INPUT_PULLDOWN);
        pinMode(AUDIO_GATE_4,INPUT_PULLDOWN);

    }
    bool read(int channel){
        return _gates[channel].read();
    }
    private:
    GpioDriver _gates[4];
};