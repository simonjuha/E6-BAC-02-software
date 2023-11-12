#pragma once
#include "ledDriver/ledDriver.hpp"

class MenuSelectionLeds
{
    public:
    MenuSelectionLeds() : _leds {LedDriver(45), LedDriver(42), LedDriver(38), LedDriver(47), LedDriver(21)}{
    }
    void select(int channel){
        for(int i = 0; i < 5; i++){
            if(i == channel){
                _leds[i].set(true);
            }else{
                _leds[i].set(false);
            }
        }
    }
    private:
    LedDriver _leds[5];
};