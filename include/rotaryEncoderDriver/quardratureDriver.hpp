#include <Arduino.h>
#include "rotaryEncoderDriver/IRotary.hpp"

#define QUADRATURE0_CLK 13
#define QUADRATURE0_DT 14


namespace Quad{
     class Quardrature : public RotarySubject{
        public:
        // pass function pointer (one argument is Quardrature pointer)
            Quardrature(int clk, int dt){
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

                gpio_isr_handler_add(_clk, ISR_clk, this); // add isr handler
                gpio_intr_enable(_clk); // enable gpio interrupt
               
            }
            void checkChange(){
                // check if clk has changed since last check, if it has check if clk == dt, if so up, else down.
                clkNewState = gpio_get_level(_clk);
                if(clkNewState != clkLastState){
                    if(clkNewState == gpio_get_level(_dt)){
                        Up();
                    }
                    else
                    {
                        Down();
                    }
                    clkLastState = clkNewState;
                }
            }
            static void ISR_clk(void* arg){
                if(arg){ // if arg is not null
                    Quardrature *instance = static_cast<Quardrature*>(arg); // get this instance.
                    instance->checkChange();

                }
            }
        private:
            gpio_num_t _clk;
            gpio_num_t _dt;
            bool clkLastState = false;
            bool clkNewState = false;
    };
}





