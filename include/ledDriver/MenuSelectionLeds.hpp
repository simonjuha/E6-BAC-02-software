#pragma once
#include "ledDriver/ledDriver.hpp"

class MenuSelectionLeds
{
    public:
    MenuSelectionLeds() : _leds {new LedDriver(45), new LedDriver(42), new LedDriver(38), new LedDriver(47), new LedDriver(21)}{
    }
    void select(int channel){
        for(int i = 0; i < 5; i++){
            if(i == channel){
                _leds[i]->set(true);
            }else{
                _leds[i]->set(false);
            }
        }
    }
    ILed* getLed(int channel){
        return _leds[channel];
    }
    private:
    ILed *_leds[5];
};