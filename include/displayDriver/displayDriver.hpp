#pragma once
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <vector>

#define SSD1306_ADDRESS 0x3C
#define SSD1306_SCREEN_WIDTH 128
#define SSD1306_SCREEN_HEIGHT 64
#define SSD1306_SDA_PIN 20
#define SSD1306_SCL_PIN 19





// singleton
class DisplayDriver{
    public:
    static DisplayDriver& getInstance(){
        static DisplayDriver instance;
        return instance;
    }

    void init(){
        if(Wire.begin(SSD1306_SDA_PIN, SSD1306_SCL_PIN)){
            ESP_LOGI("DisplayDriver", "Wire.begin() succeeded");
        }else{
            ESP_LOGE("DisplayDriver", "Wire.begin() failed");
        }
        _display = Adafruit_SSD1306(SSD1306_SCREEN_WIDTH, SSD1306_SCREEN_HEIGHT, &Wire, -1);
        _display.begin(SSD1306_SWITCHCAPVCC, SSD1306_ADDRESS);
        _display.setTextColor(WHITE);
        _display.setTextSize(1);
        _display.setRotation(2);
        _display.clearDisplay();
    }

    void writeLine(String inputText, bool selected = false){
        _display.setCursor(0,line);
        _display.println(inputText);
        if(selected){
            _display.setTextColor(BLACK, WHITE); // 'inverted' text
        }else{
            _display.setTextColor(WHITE);
        }
        _display.display();
        line += 10;
        if(line > 60){
            line = 0;
            _display.clearDisplay();
        }
    }

    void rewriteLine(std::string newLine, int lineNum){
        _display.fillRect(0,lineNum*10,128,10,BLACK);
        _display.setCursor(0,lineNum*10);
        _display.println(newLine.c_str());
        _display.display();
    }

    void clear(){
        _display.clearDisplay();
        line = 0;
        _display.setCursor(0,0);
    }

    private:
    DisplayDriver(){}
    DisplayDriver(DisplayDriver const&) = delete;
    DisplayDriver& operator=(DisplayDriver const&) = delete;
    int line = 0;
    const int _maxLines = 6;
    Adafruit_SSD1306 _display;
};



