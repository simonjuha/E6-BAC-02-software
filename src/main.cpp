/*
    ESP32-S3-DevKitC-1-N8R8 (8 MB)
*/
/*
#include <Arduino.h>

#include "cvDriver/adcDriver.hpp"
#include "Arduino.h"
#include <string>
*/
#include <RTOS.h>
#include "booleanDriver/gpioDriver.hpp"
#include <string>
#include "rotaryEncoderDriver/quardratureDriver.hpp"
#include "menuStructure/parameters.hpp"
#include "audioDriver/audioDriver.hpp"
#include "sdCardDriver/sdCardDriver.hpp"
#include "ledDriver/ledDriver.hpp"

#include "displayDriver/displayDriver.hpp"
#include "menuStructure/menu.hpp"
#include "ledDriver/menuSelectionLeds.hpp"

#include "rotaryEncoderDriver/rotaryTypes.hpp"
#include "booleanDriver/boolEdge.hpp"

#include "menuStructure/audioChannelButton.hpp"
MenuUI globalMenu;
MenuUI menuUI1;
MenuUI menuUI2;
MenuUI menuUI3;
MenuUI menuUI4;
MenuSelector menuSelector;

void userCore(void * parameter){
    MenuSelectionLeds leds;

    ValueRotary     rotaryValue(&menuSelector);
    SelectorRotary  rotarySelect(&menuSelector);
    
    GpioDriver globalButton(39, PullDirection::UP);
    GpioDriver button1(41, PullDirection::UP);
    GpioDriver button2(40, PullDirection::UP);
    GpioDriver button3(48, PullDirection::UP);
    GpioDriver button4(14, PullDirection::UP);

    AudioChannelButton globalMenuButton(0,    &globalButton, &menuSelector, &leds);
    AudioChannelButton audioChannelButton1(1, &button1, &menuSelector, &leds);
    AudioChannelButton audioChannelButton2(2, &button2, &menuSelector, &leds);
    AudioChannelButton audioChannelButton3(3, &button3, &menuSelector, &leds);
    AudioChannelButton audioChannelButton4(4, &button4, &menuSelector, &leds);


    while (true){
        globalMenuButton.tick();
        audioChannelButton1.tick();
        audioChannelButton2.tick();
        audioChannelButton3.tick();
        audioChannelButton4.tick();
        rotaryValue.tick();
        rotarySelect.tick();
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void audioCore(void * parameter){

    while(true){
        AudioDriver::getInstance().tick();
        //vTaskDelay(1 / portTICK_PERIOD_MS);

    }
}

void setup() {
    Serial.begin(115200);
    // set pin 17 to high

    DisplayDriver::getInstance().init();
    SdCardDriver::getInstance().init();
    AudioDriver::getInstance().init();
   
    menuSelector.addMenu(&globalMenu);
    menuSelector.addMenu(&menuUI1);
    menuSelector.addMenu(&menuUI2);
    menuSelector.addMenu(&menuUI3);
    menuSelector.addMenu(&menuUI4);

    // get sample names from SD card
    auto sampleNames =      std::make_shared<std::vector<std::string>>(SdCardDriver::getInstance().getSampleNames());
    auto algorithNames =    std::make_shared<std::vector<std::string>>(std::vector<std::string>{"forward", "backward", "pingpong","granular"});
    auto gateModeNames =    std::make_shared<std::vector<std::string>>(std::vector<std::string>{"hold", "once", "loop"});


    // common parameters for channels
    FloatParameter    volume =    {"volume", 0, 1, 1.0};
    OptionParameter sample =    {"sample", sampleNames, 0};
    OptionParameter algorithm = {"playback", algorithNames, 0};
    OptionParameter gateMode = {"gateMode", gateModeNames, 0};

    // global parameters
    IntParameter    sampleRate ={"sampleRate", 1, 124000, 44100, 500 };
    FloatParameter  globalVolume = {"globalVolume", 0, 2, 1.0};
    auto global_samplerate = std::make_shared<IntParameter>(sampleRate);
    auto global_volume = std::make_shared<FloatParameter>(globalVolume);
    globalMenu.addUIElement(global_samplerate);
    globalMenu.addUIElement(global_volume);
    global_samplerate->attach("sampleRate", &AudioDriver::getInstance());
    global_volume->attach("globalVolume", &AudioDriver::getInstance());
    
    auto volume_channel1 = std::make_shared<FloatParameter>(volume);
    auto volume_channel2 = std::make_shared<FloatParameter>(volume);
    auto volume_channel3 = std::make_shared<FloatParameter>(volume);
    auto volume_channel4 = std::make_shared<FloatParameter>(volume);
    menuUI1.addUIElement(volume_channel1);
    menuUI2.addUIElement(volume_channel2);
    menuUI3.addUIElement(volume_channel3);
    menuUI4.addUIElement(volume_channel4);
    volume_channel1->attach("volume", &AudioDriver::getInstance());
    volume_channel2->attach("volume", &AudioDriver::getInstance());
    volume_channel3->attach("volume", &AudioDriver::getInstance());
    volume_channel4->attach("volume", &AudioDriver::getInstance());

    auto sample_channel1 = std::make_shared<OptionParameter>(sample);
    auto sample_channel2 = std::make_shared<OptionParameter>(sample);
    auto sample_channel3 = std::make_shared<OptionParameter>(sample);
    auto sample_channel4 = std::make_shared<OptionParameter>(sample);
    menuUI1.addUIElement(sample_channel1);
    menuUI2.addUIElement(sample_channel2);
    menuUI3.addUIElement(sample_channel3);
    menuUI4.addUIElement(sample_channel4);
    sample_channel1->attach("sample", &AudioDriver::getInstance());
    sample_channel2->attach("sample", &AudioDriver::getInstance());  
    sample_channel3->attach("sample", &AudioDriver::getInstance());
    sample_channel4->attach("sample", &AudioDriver::getInstance());

    auto algorithm_channel1 = std::make_shared<OptionParameter>(algorithm);
    auto algorithm_channel2 = std::make_shared<OptionParameter>(algorithm);
    auto algorithm_channel3 = std::make_shared<OptionParameter>(algorithm);
    auto algorithm_channel4 = std::make_shared<OptionParameter>(algorithm);
    menuUI1.addUIElement(algorithm_channel1);
    menuUI2.addUIElement(algorithm_channel2);
    menuUI3.addUIElement(algorithm_channel3);
    menuUI4.addUIElement(algorithm_channel4);
    algorithm_channel1->attach("playback", &AudioDriver::getInstance());
    algorithm_channel2->attach("playback", &AudioDriver::getInstance());
    algorithm_channel3->attach("playback", &AudioDriver::getInstance());
    algorithm_channel4->attach("playback", &AudioDriver::getInstance());

    auto gateMode_channel1 = std::make_shared<OptionParameter>(gateMode);
    auto gateMode_channel2 = std::make_shared<OptionParameter>(gateMode);
    auto gateMode_channel3 = std::make_shared<OptionParameter>(gateMode);
    auto gateMode_channel4 = std::make_shared<OptionParameter>(gateMode);
    menuUI1.addUIElement(gateMode_channel1);
    menuUI2.addUIElement(gateMode_channel2);
    menuUI3.addUIElement(gateMode_channel3);
    menuUI4.addUIElement(gateMode_channel4);
    gateMode_channel1->attach("gateMode", &AudioDriver::getInstance());
    gateMode_channel2->attach("gateMode", &AudioDriver::getInstance());
    gateMode_channel3->attach("gateMode", &AudioDriver::getInstance());
    gateMode_channel4->attach("gateMode", &AudioDriver::getInstance());

    globalMenu.refresh();

    // setup RTOS tasks
    xTaskCreatePinnedToCore(userCore, "userCore", 10000, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(audioCore, "audioCore", 10000, NULL, 2, NULL, 1);

}

void loop() {
    delay(1000);
}
