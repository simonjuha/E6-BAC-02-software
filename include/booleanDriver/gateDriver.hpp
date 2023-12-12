#pragma once
#include "booleanDriver/gpioDriver.hpp"
#include "booleanDriver/boolEdge.hpp"
#include "Arduino.h"

#define AUDIO_GATE_1 46
#define AUDIO_GATE_2 3
#define AUDIO_GATE_3 8
#define AUDIO_GATE_4 18

// gate inputs used by the GateInputs class
class GateObserver : public IEdgeObserver
{
    public:
    GateObserver(int channel) : _channel(channel)
    {
    }
    // risingEdge: return true if edge is rising, false is nothing happened 
    bool risingEdge(){ 
        bool rise = _rise;
        _rise = false;
        return rise;
    }
    void rise(){ // set flag        
        _rise = true;
    }
    void fall(){
        // nothing
    }
    private:
    int _channel;
    bool _rise = false;
};

class GateInputs
{
    public:
    GateInputs() : 
        _gates({AUDIO_GATE_1,AUDIO_GATE_2,AUDIO_GATE_3,AUDIO_GATE_4}), 
        _gateObservers({0,1,2,3}),          // static gate inputs
        _isRising{false,false,false,false}  // rising edge flags
    {
        pinMode(AUDIO_GATE_1,INPUT_PULLDOWN);
        pinMode(AUDIO_GATE_2,INPUT_PULLDOWN);
        pinMode(AUDIO_GATE_3,INPUT_PULLDOWN);
        pinMode(AUDIO_GATE_4,INPUT_PULLDOWN);
        for(int i = 0; i < 4; i++){
            _gates[i].attach(&_gateObservers[i]); // attach gate observers to gate inputs
        }
    }
    // read: read gate by channel index
    bool read(int channel){
        return _gates[channel].read();
    }
    // risingEdge: return true if edge is rising, false is nothing happened
    bool risingEdge(int channel){
        return _gateObservers[channel].risingEdge();
    }
    // tick: call tick method of all gate observers (IEdegeObserver)
    void tick(){
        for(int i = 0; i < 4; i++){
            _gateObservers[i].tick();
        }
    }
    private:
    GpioDriver _gates[4];
    GateObserver _gateObservers[4];
    bool _isRising[4];
};

