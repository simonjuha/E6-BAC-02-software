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
        if(!_display.begin(SSD1306_SWITCHCAPVCC, SSD1306_ADDRESS)){
            ESP_LOGE("DisplayDriver", "SSD1306 allocation failed");
        }
        _display.setTextColor(WHITE);
        _display.setTextSize(1);
        _display.setRotation(2);
        _display.clearDisplay();
    }

    void writeFixedLines(std::vector<std::string> lines, int selectedLine){
        int nLinesToPrint = std::min(_maxLines, static_cast<int>(lines.size()));
        for(int i=0; i < nLinesToPrint; i++){
                std::string lineToPrint;
                if(selectedLine == i){
                    _display.fillRect(0,i*_lineHeight-2,_displayWidth,_lineHeight+1,WHITE);
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
        }

        //_displayLines = lines;
        _display.display();
    }

    void select(int sel){
        if(sel < 0 || sel >= _lines.size()){
            ESP_LOGW("DisplayDriver", "Invalid line selection");
            return;
        }
        _selectedLine = sel;
        refreshPositions();
        writeRelativeLines();
    }

    void writeRelativeLines(){
    auto startIt = _lines.begin() + _scroll;
    auto endIt = (_lines.size() < _maxLines) ? _lines.end() : startIt + _maxLines;
    std::vector<std::string> fixedLines(startIt, endIt);
        writeFixedLines(fixedLines, _relativeSelectedLine);
    }

    void setLines(std::vector<std::string> lines){
        if (lines.size() == 0)
        {
            ESP_LOGW("DisplayDriver", "No lines to display");
            return;
        }
        _lines = lines;
    }
    
    void clear(){
        _display.clearDisplay();
        _display.setCursor(0,0);
    }

    private:
    // recalculate the position based on seleect display height vector length etc.
void refreshPositions() {
    _listLines = _lines.size(); // Get the total number of lines
    _scroll = 0; // Default scroll to 0
    _relativeSelectedLine = _selectedLine; // By default, the relative selected line is the same as the selected line

    // If the number of lines is less than _maxLines, no need to scroll or adjust
    if (_listLines > _maxLines) {
        // If the selected line is beyond the visible display
        if (_selectedLine >= _maxLines) {
            _scroll = _selectedLine - _maxLines + 1; // Calculate scroll position
            _relativeSelectedLine = _maxLines - 1; // Set the relative selected line to the last visible line
        } else {
            // If the selected line is within the visible display, no need to scroll
            _scroll = 0;
            _relativeSelectedLine = _selectedLine;
        }

        // If the scroll is beyond the list length, adjust it
        if (_scroll > _listLines - _maxLines) {
            _scroll = _listLines - _maxLines;
        }
    }

    // Ensure the relative selected line is within bounds
    _relativeSelectedLine = std::min(_relativeSelectedLine, _maxLines - 1);
    _relativeSelectedLine = std::max(0, _relativeSelectedLine); // Ensure it's not negative
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



