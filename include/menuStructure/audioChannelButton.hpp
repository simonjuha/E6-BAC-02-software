#pragma once
#include <memory>
#include <vector>
#include "booleanDriver/boolEdge.hpp"
#include "menuStructure/menu.hpp"
#include "audioDriver/audioDriver.hpp"

class AudioChannelButton : public IEdgeObserver{
    public:
    AudioChannelButton(int channelNumber, EdgeSubject * button, MenuSelector* menu) : 
        _channelNumber(channelNumber), 
        _menu(menu)
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
    }
    private:
    int _channelNumber;
    MenuSelector* _menu;
};