#include <esp32-hal-gpio.h>
#include "booleanDriver/IBool.hpp"
#include "booleanDriver/boolEdge.hpp"

class gpioDriver : public IBool, public EdgeSubject{
    public :
    gpioDriver(int pin, bool pullDown = false, bool pullUp = false){
        _pinNum = static_cast<gpio_num_t>(pin);

        // interrupt on gpio setup (CHANGE)
        gpio_config_t pin_config;
        pin_config.pin_bit_mask = (1ULL<<_pinNum);
        pin_config.mode = GPIO_MODE_INPUT;
        pin_config.pull_up_en = pullUp ? GPIO_PULLUP_ENABLE : GPIO_PULLUP_DISABLE;
        pin_config.pull_down_en = pullDown ? GPIO_PULLDOWN_ENABLE : GPIO_PULLDOWN_DISABLE;
        pin_config.intr_type = GPIO_INTR_ANYEDGE; // trigger on any edge
        gpio_config(&pin_config);

        // gpio install once
        static bool isr_installed = false;
        if(!isr_installed){
            gpio_install_isr_service(ESP_INTR_FLAG_IRAM); // install gpio isr service. this should be called only once by moving it into 
            isr_installed = true;
        }
        gpio_isr_handler_add(_pinNum, ISR_gpio, this); // add isr handler
        gpio_intr_enable(_pinNum); // enable gpio interrupt

    }
    bool read(){
        return gpio_get_level(_pinNum); // simple read from IBool
    }
    bool triggered;
    private :
    gpio_num_t _pinNum;
    static void ISR_gpio(void* arg){
        if(arg){ // if arg is not null
            gpioDriver *instance = static_cast<gpioDriver*>(arg); // get this instance
            instance->triggered = true; // set triggered to true
            // check if falling or rising edge. use fall() or rise() accordingly
            if(instance->read()){
                instance->rise();
            }else{
                instance->fall();
            }
        }
    }
    
};