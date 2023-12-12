#pragma once
#include "ledDriver/ledDriver.hpp"

class MenuSelectionLeds
{
    public:
    MenuSelectionLeds() : _leds {new LedDriver(45), new LedDriver(42), new LedDriver(38), new LedDriver(47), new LedDriver(21)}{ // setting up static leds
    }
    void select(int channel){
        for(int i = 0; i < 5; i++){
            if(i == channel){
                _leds[i]->set(true); // set selected led to true
            }else{
                _leds[i]->set(false); // if not selected, turn off
            }
        }
    }
    ILed* getLed(int channel){
        return _leds[channel]; // return led by channel index
    }
    private:
    ILed *_leds[5]; // array of leds objects
};