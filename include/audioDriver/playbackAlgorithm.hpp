#include <Arduino.h>
#include <SPI.h>
#include "sdCardDriver/sample.hpp"

// playbackAlgorithm interface
class playbackAlgorithm
{
    public:
    virtual void play(Sample &sample, SPIClass *vspi) = 0;
};

class forwardAlgorithm : public playbackAlgorithm
{
    public:
    void play(Sample &sample, SPIClass *vspi)
    {
        static unsigned int currentPosition = 0;
        if(currentPosition > sample.size-1){
            currentPosition = 0;
        }
        vspi->transfer(sample.buffer[currentPosition]);
        currentPosition++;
    }

};

class backwardAlgorithm : public playbackAlgorithm
{
    public:
    void play(Sample &sample, SPIClass *vspi)
    {
        static unsigned int currentPosition = sample.size;
        if(currentPosition == 0){
            currentPosition = sample.size;
        }
        vspi->transfer(sample.buffer[currentPosition]);
        currentPosition--;
    }

};