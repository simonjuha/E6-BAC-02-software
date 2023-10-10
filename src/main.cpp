/*
    ESP32-S3-DevKitC-1-N8R8 (8 MB)
*/
#include <Arduino.h>
//#include "rotaryEncoderDriver/rotaryEncoderDriver.hpp"
#include "rotaryEncoderDriver/quardratureDriver.hpp"
#include "booleanDriver/gpioDriver.hpp"
// measure time lib (C++)
#include <chrono>
#include <RTOS.h>



class testReciver : public IRotary, public IEdgeObserver{
    public:
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
                Serial.println("up");
                upFlag = false;
            }
            if(downFlag){
                Serial.println("down");
                downFlag = false;
            }
            if(riseFlag){
                Serial.println("rise");
                riseFlag = false;
            }
            if(fallFlag){
                Serial.println("fall");
                fallFlag = false;
            }
        }
        bool riseFlag = false;
        bool fallFlag = false;
        bool upFlag   = false;
        bool downFlag = false;
};

void setup() 
{
    Serial.begin(115200);
    
    xTaskCreate([](void *pvParameters){
        testReciver t;
        Quad::initQuadratureInterrupt();
        Quad::q0.setRotary(&t);
        while(1){
            t.tick();
            vTaskDelay(10 / portTICK_PERIOD_MS);
            Quad::q0.print();
        }
    }, "rotary", 10000, NULL, 1, NULL);
    

    xTaskCreate([](void *pvParameters){
        gpioDriver d = gpioDriver(17);
        testReciver t;
        d.attach(&t);          
        while(1){
            vTaskDelay(10 / portTICK_PERIOD_MS); 
            t.tick();
        }
    }, "pushswitch", 10000, NULL, 1, NULL);

}


void loop() 
{
    
}