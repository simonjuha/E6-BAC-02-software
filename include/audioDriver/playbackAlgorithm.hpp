#include <Arduino.h>
#include <SPI.h>
#include "sdCardDriver/sample.hpp"

// playbackAlgorithm interface
class playbackAlgorithm
{
    public:
    virtual void process(Sample *sample, SPIClass *vspi) = 0;
};

class forwardAlgorithm : public playbackAlgorithm
{
    public:
    void process(Sample *sample, SPIClass *vspi)
    {
        static int currentPosition = 0;
        vspi->transfer(sample->buffer[currentPosition]);
        if(currentPosition++ > sample->size){
            currentPosition = 0;
        }
    }

};