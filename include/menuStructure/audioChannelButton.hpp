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
        AudioDriver::getInstance().selectChannel(_channelNumber);
        _menu->select(_channelNumber);
    }
    void fall() override{

    }
    private:
    int _channelNumber;
    MenuSelector* _menu;
};