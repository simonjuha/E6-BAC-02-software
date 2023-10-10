#include <Arduino.h>
#include "rotaryEncoderDriver/IRotary.hpp"

#define QUADRATURE0_CLK 13
#define QUADRATURE0_DT 14


namespace Quad{
     class Quardrature{
        public:
        // pass function pointer (one argument is Quardrature pointer)
            Quardrature(int clk, int dt) : clk(clk), dt(dt){
                pinMode(clk, INPUT_PULLDOWN);
                pinMode(dt, INPUT_PULLDOWN);
            }
            void checkChange(){
                // check if clk has changed since last check, if it has check if clk == dt, if so up, else down.
                clkNewState = digitalRead(clk);
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

            IRotary *rot; // pointer to rotary object
            bool up = false;
            bool down = false;
            int clk;
            int dt;
            bool clkLastState = false;
            bool clkNewState = false;
    };

    // hardcodes setup instead of class
    Quardrature q0 = Quardrature(QUADRATURE0_CLK, QUADRATURE0_DT);
    Quardrature q1 = Quardrature(QUADRATURE0_CLK, QUADRATURE0_DT);
    
    void quadratureInterrupt0(){
        q0.checkChange();

    }

    void quadratureInterrupt1(){
        //q1.checkChange();
    }

    void initQuadratureInterrupt(){
        attachInterrupt(digitalPinToInterrupt(q0.clk), quadratureInterrupt0, CHANGE);
        //attachInterrupt(digitalPinToInterrupt(q1.clk), quadratureInterrupt1, CHANGE);
    }

    void setRotary0(IRotary *rot){
        q0.rot = rot;
    }

    void setRotary1(IRotary *rot){
        q1.rot = rot;
    }

}





