/*
    ESP32-S3-DevKitC-1-N8R8 (8 MB)
*/
#include <Arduino.h>
#include <RTOS.h>

#include "rotaryEncoderDriver/quardratureDriver.hpp"
#include "booleanDriver/gpioDriver.hpp"
#include "displayDriver/displayDriver.hpp"

 

class testReciver : public IRotaryObserver, public IEdgeObserver{
    public:
        testReciver(String name){
            _name = name;
        }
        void up(){
            upFlag = true;
        }
        void down(){
            downFlag = true;
        }
        void rise(){
            riseFlag = true;
        }
        void fall(){
            fallFlag = true;
        }
        void tick(){
            if(upFlag){
                //display.writeLine(_name + ": up");
                Serial.println(_name + ": up");
                upFlag = false;
            }
            if(downFlag){
                //display.writeLine(_name + ": down");
                Serial.println(_name + ": down");
                downFlag = false;
            }
            if(riseFlag){
                //display.writeLine(_name + ": rise");
                Serial.println(_name + ": rise");
                riseFlag = false;
            }
            if(fallFlag){
                //display.writeLine(_name + ": fall");
                Serial.println(_name + ": fall");
                fallFlag = false;
            }
        }
        bool riseFlag = false;
        bool fallFlag = false;
        bool upFlag   = false;
        bool downFlag = false;
        String _name;
};

void setup() 
{
    
    Serial.begin(115200);


    
     gpio_install_isr_service(ESP_INTR_FLAG_IRAM);
    xTaskCreate([](void *pvParameters){
        delay(1000);
        DisplayDriver display;
            display.writeLine("Hello World");
    display.writeLine("new line");
    display.writeLine("another new line");
    display.writeLine("another new line");
    display.writeLine("another new line");
        testReciver t1("t1");
        testReciver t2("t2");
        Quad::Quardrature q0(QUADRATURE0_CLK, QUADRATURE0_DT);
        Quad::Quardrature q1(4, 5);
       
        
        //Quad::initQuadratureInterrupt();
        q0.attach(&t1);
        q0.attach(&t2);
        q1.attach(&t1);
        q1.attach(&t2);
        
 

        while(1){
            vTaskDelay(10 / portTICK_PERIOD_MS);
            if(t1.downFlag){
                display.writeLine("t1: down");
                t1.downFlag = false;
            }
            if(t1.upFlag){
                display.writeLine("t1: up");
                t1.upFlag = false;
            }
            if(t1.riseFlag){
                display.writeLine("t1: rise");
                t1.riseFlag = false;
            }
            if(t1.fallFlag){
                display.writeLine("t1: fall");
                t1.fallFlag = false;
            }
            t1.tick();
            t2.tick();
        }
    }, "rotary", 10000, NULL, 1, NULL);
    

    xTaskCreate([](void *pvParameters){
        gpioDriver d = gpioDriver(17);
        gpioDriver d7 = gpioDriver(7);
        testReciver t3("t3");
        testReciver t4("t4");


        d.attach(&t3);
        d7.attach(&t3);
        d.attach(&t4);
        d7.attach(&t4);

        while(1){
            vTaskDelay(10 / portTICK_PERIOD_MS); 
            t3.tick();
            t4.tick();
        }
    }, "pushswitch", 10000, NULL, 1, NULL);

}


void loop() 
{
    
}