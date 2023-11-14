#pragma once
#include <esp32-hal-gpio.h>
#include "booleanDriver/IBool.hpp"
#include "booleanDriver/boolEdge.hpp"

#define GPIO_DEBOUNCE 5

enum PullDirection{
    UP,
    DOWN
};

class GpioDriver : public IBool, public EdgeSubject{
    public :
    GpioDriver(int pin, int pullDirection = PullDirection::DOWN){
        _pinNum = static_cast<gpio_num_t>(pin);

        // interrupt on gpio setup (CHANGE)
        gpio_config_t pin_config;
        pin_config.pin_bit_mask = (1ULL<<_pinNum);
        pin_config.mode = GPIO_MODE_INPUT;
        pin_config.pull_up_en = (pullDirection == PullDirection::UP) ? GPIO_PULLUP_ENABLE : GPIO_PULLUP_DISABLE;
        pin_config.pull_down_en = (pullDirection == PullDirection::DOWN) ? GPIO_PULLDOWN_ENABLE : GPIO_PULLDOWN_DISABLE;
        pin_config.intr_type = GPIO_INTR_ANYEDGE; // trigger on any edge
        gpio_config(&pin_config);
        gpio_install_isr_service(0); // install isr service
        gpio_isr_handler_add(_pinNum, ISR_gpio, this); // add isr handler
        gpio_intr_enable(_pinNum); // enable gpio interrupt
        

    }
    bool read(){
        return gpio_get_level(_pinNum); // simple read from IBool
    }
    void checkEdge(){
        if(read()){
            rise();
        }
        else{
            fall();
        }
    }
    private :
    gpio_num_t _pinNum;
    static void ISR_gpio(void* arg){
        if(arg){ // if arg is not null
            // debounce
            #if GPIO_DEBOUNCE > 0
                static double lastTime = 0;
                double time = millis();
                if(time - lastTime < GPIO_DEBOUNCE){
                    return; // ignore interrupt
                }
                lastTime = time;
            #endif
            GpioDriver *instance = static_cast<GpioDriver*>(arg); // get this instance.
            instance->checkEdge();
        }
    }
    
};