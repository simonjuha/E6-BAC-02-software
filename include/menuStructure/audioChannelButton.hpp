#pragma once
#include <memory>
#include <vector>
#include "booleanDriver/boolEdge.hpp"
#include "menuStructure/menu.hpp"
#include "audioDriver/audioDriver.hpp"

class AudioChannelButton : public IEdgeObserver{
    public:
    AudioChannelButton(int channelNumber, std::shared_ptr<AudioDriver> driver, EdgeSubject * button, MenuSelector* menu) : 
        _channelNumber(channelNumber), 
        _driver(driver),
        _menu(menu)
    {
        button->attach(this);
    }
    void rise() override{
        ESP_LOGI("AudioChannelButton", "Channel %d selected", _channelNumber);
        _driver->selectChannel(_channelNumber);
        _menu->select(_channelNumber);
    }
    void fall() override{
        ESP_LOGI("AudioChannelButton", "Channel %d selected", _channelNumber);
        _driver->selectChannel(_channelNumber);
        _menu->select(_channelNumber);
    }
    private:
    int _channelNumber;
    std::shared_ptr<AudioDriver> _driver;
    MenuSelector* _menu;
};