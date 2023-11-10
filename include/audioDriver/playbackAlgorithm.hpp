#include <Arduino.h>
#include <SPI.h>
#include "sdCardDriver/sample.hpp"

// playbackAlgorithm interface
class playbackAlgorithm
{
    public:
    virtual int16_t & play(Sample &sample) = 0;
    private: unsigned int currentPositon = 0;
};

class forwardAlgorithm : public playbackAlgorithm
{
    public:
    unsigned int currentPosition = 0;
    int16_t & play(Sample &sample)
    {
        if(currentPosition >= sample.size){
            currentPosition = 0;
        }
        return sample.buffer[++currentPosition];
    }
};

class backwardAlgorithm : public playbackAlgorithm
{
    public:
    unsigned int currentPosition = 0;
    int16_t & play(Sample &sample)
    {
        if(currentPosition <= 0){
            currentPosition = sample.size;
        }
        return sample.buffer[--currentPosition];
    }
};