#pragma once
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <vector>
#include <string>
#include <algorithm>

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

    void writeFixedLines(std::vector<std::string> lines, int selectedLine){
        //static std::vector<std::string> oldLines;

        for(int i=0; i < _maxLines; i++){
            std::string lineToPrint;
            if(selectedLine == i){
                _display.fillRect(0,i*_lineHeight,_displayWidth,_lineHeight,WHITE);
                _display.setTextColor(BLACK);
                lineToPrint = ">" + lines[i];
            }
            else{
                _display.fillRect(0,i*_lineHeight,_displayWidth,_lineHeight,BLACK);
                _display.setTextColor(WHITE);
                lineToPrint = " " + lines[i];
            }
            _display.setCursor(0, i*_lineHeight);
            _display.println(lineToPrint.c_str());
            //oldLines[i] = lines[i];
        }

        _display.display();
    }

    void select(int sel){
        if(sel < 0 || sel > _lines.size()){
            return;
        }
        _selectedLine = sel;
        refreshPositions();
        writeRelativeLines();
    }

    void writeRelativeLines(){
        std::vector<std::string> fixedLines = {
            _lines.begin() + _scroll,
            _lines.begin() + _scroll + _maxLines
        };
        writeFixedLines(fixedLines, _relativeSelectedLine);
    }

    void setLines(std::vector<std::string> lines){
        _lines = lines;
        _selectedLine = 0;
        refreshPositions();
        writeRelativeLines();
    }
    
    void clear(){
        _display.clearDisplay();
        _display.setCursor(0,0);
    }

    private:
    // recalculate the position based on seleect display height vector length etc.
    void refreshPositions(){
    _listLines = _lines.size();
    if(_selectedLine >= _maxLines) // if selected line is beyond the visible display
    {
        _scroll = _selectedLine - _maxLines + 1;
        _relativeSelectedLine = _maxLines - 1;
    }
    else // if selected line is within the visible display
    {
        _scroll = 0;
        _relativeSelectedLine = _selectedLine;
    }

    // if the scroll is beyond the list length
    if (_scroll > _listLines - _maxLines) {
        _scroll = _listLines - _maxLines;
    }

}

    DisplayDriver(){}
    DisplayDriver(DisplayDriver const&) = delete;
    DisplayDriver& operator=(DisplayDriver const&) = delete;
    int _selectedLine = 0;
    int _listLines = 0;
    int _relativeSelectedLine = 0; // 0-5 display select
    int _scroll = 0;
    const int _maxLines = 6;
    const int _lineHeight = 10;
    const int _displayHeight = 64;
    const int _displayWidth = 128;
    std::vector<std::string> _lines;
    Adafruit_SSD1306 _display;
};



