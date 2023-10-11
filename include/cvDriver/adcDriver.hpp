#include "cvDriver/ControlSignal.hpp"
#include <esp32-hal-adc.h>
#include <esp_adc_cal.h>


class adcDriver : public IControlSignalRead{
    public:
    adcDriver(adc1_channel_t adcChannel) : _adcChannel(adcChannel){
        adc1_pad_get_io_num(_adcChannel,&_pin); // get pin from adc channel
        adc1_config_width(ADC_WIDTH_BIT_12);
        adc1_config_channel_atten(_adcChannel, ADC_ATTEN_DB_11); // get 0-3.3V range
    }
    // read from adc <<interface>>
    int read(){
        return adc1_get_raw(_adcChannel);
    }
    private:
    gpio_num_t _pin;
    adc1_channel_t _adcChannel;
};