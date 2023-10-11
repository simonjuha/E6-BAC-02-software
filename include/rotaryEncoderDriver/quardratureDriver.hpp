#include <Arduino.h>
#include "rotaryEncoderDriver/IRotary.hpp"

#define QUADRATURE0_CLK 13
#define QUADRATURE0_DT 14


namespace Quad{
     class Quardrature{
        public:
        // pass function pointer (one argument is Quardrature pointer)
            Quardrature(int clk, int dt) : dt(dt){
                _clk = static_cast<gpio_num_t>(clk);
                //pinMode(clk, INPUT_PULLDOWN);
                pinMode(dt, INPUT_PULLDOWN);

                // interrupt on gpio setup (CHANGE)
                gpio_config_t pin_config;
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
                    if(clkNewState == digitalRead(dt)){
                        up = true;
                    }
                    else
                    {
                        down = true;
                    }
                }
                clkLastState = clkNewState;
            }
            void setRotary(IRotary *rot){
                this->rot = rot;
            }
            void print(){
                if(up){
                    rot->up();
                    up = false;
                }
                if(down){
                    rot->down();
                    down = false;
                }
            }
            static void ISR_clk(void* arg){
                if(arg){ // if arg is not null
                    Quardrature *instance = static_cast<Quardrature*>(arg); // get this instance.
                    instance->checkChange();

                }
            }

            IRotary *rot; // pointer to rotary object
            bool up = false;
            bool down = false;
            gpio_num_t _clk;
            int dt;
            bool clkLastState = false;
            bool clkNewState = false;
    };

    // hardcodes setup instead of class
    /*
    Quardrature q0 = Quardrature(QUADRATURE0_CLK, QUADRATURE0_DT);
    Quardrature q1 = Quardrature(QUADRATURE0_CLK, QUADRATURE0_DT);
    
    void quadratureInterrupt0(){
        q0.checkChange();

    }

    void quadratureInterrupt1(){
        //q1.checkChange();
    }

    void initQuadratureInterrupt(){
        //attachInterrupt(digitalPinToInterrupt(q0.clk), quadratureInterrupt0, CHANGE);
        //attachInterrupt(digitalPinToInterrupt(q1.clk), quadratureInterrupt1, CHANGE);
    }

    void setRotary0(IRotary *rot){
        q0.rot = rot;
    }

    void setRotary1(IRotary *rot){
        q1.rot = rot;
    }
*/
}





