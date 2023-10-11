#include <esp32-hal-gpio.h>

class ILed{
    public:
    virtual void set(bool state) = 0;
    virtual bool state() = 0;
};


class LedDriver : public ILed{
    public:
    LedDriver(int pinNum){
        _pinNum = static_cast<gpio_num_t>(pinNum);
        gpio_config_t pin_config;
        pin_config.pin_bit_mask = (1ULL<<_pinNum);
        pin_config.mode = GPIO_MODE_OUTPUT;
        pin_config.pull_up_en = GPIO_PULLUP_DISABLE;
        pin_config.pull_down_en = GPIO_PULLDOWN_DISABLE;
        pin_config.intr_type = GPIO_INTR_DISABLE;
        gpio_config(&pin_config);
    }
    void set(bool state){
        if(state){
            gpio_set_level(_pinNum, 1);
            _state = true;
        }else{
            gpio_set_level(_pinNum, 0);
            _state = false;
        }
    }
    bool state(){
        return _state;
    }
    private:
    gpio_num_t _pinNum;
    bool _state = false;
};