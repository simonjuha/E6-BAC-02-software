#pragma once
#include <memory>
#include <vector>
#include "booleanDriver/boolEdge.hpp"
#include "menuStructure/menu.hpp"
#include "audioDriver/audioDriver.hpp"

class AudioChannelButton : public IEdgeObserver{
    public:
    AudioChannelButton(int channelNumber, EdgeSubject * button, MenuSelector* menu, MenuSelectionLeds* leds) : 
        _channelNumber(channelNumber), 
        _menu(menu),
        _leds(leds)
    {
        button->attach(this);
    }
    void rise() override{
        // no action
    }
    void fall() override{
        // falling edge trigger

        AudioDriver::getInstance().selectChannel(_channelNumber);  
        _menu->select(_channelNumber);
        _leds->select(_channelNumber);
    }
    private:
    int _channelNumber;
    MenuSelectionLeds* _leds;
    MenuSelector* _menu;
};