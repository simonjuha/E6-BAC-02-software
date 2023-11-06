#include <Arduino.h>
#include "rotaryEncoderDriver/IRotary.hpp"

class Quadrature : public RotarySubject{
public:
// pass function pointer (one argument is Quardrature pointer)
    Quadrature(){}
    Quadrature(int clk, int dt){
        _clk = static_cast<gpio_num_t>(clk);
        _dt = static_cast<gpio_num_t>(dt);

        gpio_config_t pin_config;
        // dt pin setup
        pin_config.pin_bit_mask = (1ULL<<dt);
        pin_config.mode = GPIO_MODE_INPUT;
        pin_config.pull_up_en = GPIO_PULLUP_DISABLE;
        pin_config.pull_down_en = GPIO_PULLDOWN_ENABLE;
        pin_config.intr_type = GPIO_INTR_DISABLE;
        gpio_config(&pin_config);

        // interrupt on clk setup (CHANGE)
        pin_config.pin_bit_mask = (1ULL<<clk);
        pin_config.mode = GPIO_MODE_INPUT;
        pin_config.pull_up_en = GPIO_PULLUP_DISABLE;
        pin_config.pull_down_en = GPIO_PULLDOWN_ENABLE;
        pin_config.intr_type = GPIO_INTR_ANYEDGE; // trigger on any edge
        gpio_config(&pin_config);
        gpio_install_isr_service(0); // install isr service
        gpio_isr_handler_add(_clk, ISR_clk, this); // add isr handler
        gpio_intr_enable(_clk); // enable gpio interrupt
        
    }
    void checkChange(){
        // check if clk has changed since last check, if it has check if clk == dt, if so up, else down.
        clkNewState = gpio_get_level(_clk);
        if(clkNewState != clkLastState){
            if(clkNewState == gpio_get_level(_dt)){
                Down();
            }
            else
            {
                Up();
            }
            clkLastState = clkNewState;
        }
    }
    static void ISR_clk(void* arg){
        if(arg){ // if arg is not null
            Quadrature *instance = static_cast<Quadrature*>(arg); // get this instance.
            instance->checkChange();

        }
    }
private:
    gpio_num_t _clk;
    gpio_num_t _dt;
    bool clkLastState = false;
    bool clkNewState = false;
};






