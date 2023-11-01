/*
    ESP32-S3-DevKitC-1-N8R8 (8 MB)
*/
/*
#include <Arduino.h>

#include "booleanDriver/gpioDriver.hpp"
#include "cvDriver/adcDriver.hpp"
#include "ledDriver/ledDriver.hpp"
#include "Arduino.h"
#include <string>
*/
#include <RTOS.h>
#include <string>
#include "rotaryEncoderDriver/quardratureDriver.hpp"
#include "menuStructure/parameters.hpp"
#include "audioDriver/audioDriver.hpp"
#include "sdCardDriver/sdCardDriver.hpp"

#include "displayDriver/displayDriver.hpp"
#include "displayDriver/uiDraw.hpp"

//#include <esp32-hal-spi.h>
//#include <esp32-hal-gpio.h>
//#include <driver/spi_master.h>


SdCardDriver sdCardDriver;
AudioDriver audioDriver;
DisplayDriver displayDriver;
    Sample chirpSample, toneSample;

class RotaryObserver : public IRotaryObserver {
    public:
        void up(){
            value++;
            changed = true;
        }
        void down(){
            value--;
            changed = true;
        }
        void tick(){
            if (changed)
            {
                displayDriver.clear();
                displayDriver.writeLine(std::to_string(value).c_str());
                changed = false;
            }
        }
    private:
    int value = 0;
    bool changed = false;
};

void task1(void * parameter){
    RotaryObserver rotaryObserver;
    RotaryObserver rotaryObserver2;
    Quadrature rotarySubject(2, 1);
    Quadrature rotarySubject2(35, 36);
    rotarySubject.attach(&rotaryObserver);
    rotarySubject2.attach(&rotaryObserver2);



    
    while (true)
    {
        rotaryObserver.tick();
        rotaryObserver2.tick();
        // RTOS delay
        vTaskDelay(10 / portTICK_PERIOD_MS); // allows for some time for other tasks to run
    }
}

void task2(void * parameter){
    Sample chirpSample, toneSample;
    sdCardDriver.getSampleByName("sweep.wav",&chirpSample);
    sdCardDriver.getSampleByName("tone.wav",&toneSample);
    audioDriver.setSampleSlot(0, &chirpSample);
    audioDriver.setSampleSlot(1, &toneSample);
    while(true){
        audioDriver.tick();
    }
}



void setup() {
    Serial.begin(115200);
    IntParameter intParameter("test", 0, 100, 50);
    intParameter.accept(new DefaultDrawVisitor());
    intParameter.increment();
    intParameter.accept(new DefaultDrawVisitor());

    OptionParameter optionParameter("test", {"option1", "option2", "option3"}, 0);
    optionParameter.accept(new DefaultDrawVisitor());
    optionParameter.increment();
    optionParameter.accept(new DefaultDrawVisitor());
    optionParameter.increment();
    optionParameter.accept(new DefaultDrawVisitor());


    while (true)
    {
        /* code */
    }


    displayDriver.init();



    // setup RTOS tasks
   xTaskCreatePinnedToCore(task1, "Task1", 10000, NULL, 1, NULL, 0);
   xTaskCreatePinnedToCore(task2, "Task2", 10000, NULL, 1, NULL, 1);
}

void loop() {
    delay(1000);
}
