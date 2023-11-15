#include <Arduino.h>
#include <SPI.h>
#include "sdCardDriver/sample.hpp"

// playbackAlgorithm interface
class playbackAlgorithm
{
    public:
    virtual int16_t & play(Sample &sample) = 0;
    virtual bool isAtEnd(Sample &sample) = 0;
    virtual void reset(Sample &sample) = 0;
    protected: 
    unsigned int currentPosition = 0;
};

class forwardAlgorithm : public playbackAlgorithm
{
    public:
    int16_t & play(Sample &sample)
    {
        if(isAtEnd(sample)){
            currentPosition = 0;
        }
        return sample.buffer[++currentPosition];
    }
    bool isAtEnd(Sample &sample){
        return (currentPosition >= sample.size-1);
    }
    void reset(Sample &sample){
        currentPosition = 0;
    }
};

class backwardAlgorithm : public playbackAlgorithm
{
    public:
    unsigned int currentPosition = 0;
    int16_t & play(Sample &sample)
    {
        if(isAtEnd(sample)){
            currentPosition = sample.size-1;
        }
        return sample.buffer[--currentPosition];
    }
    bool isAtEnd(Sample &sample){
        return (currentPosition <= 0);
    }
    void reset(Sample &sample){
        currentPosition = sample.size-1;
    }
};

class pingPongAlgorithm : public playbackAlgorithm
{
    public:
    unsigned int currentPosition = 0;
    int16_t & play(Sample &sample)
    {
        if(currentPosition >= sample.size-1){
            currentPosition = sample.size-1;
            direction = false;
        }
        if(currentPosition <= 0){
            currentPosition = 0;
            direction = true;
        }
        if(direction){
            return sample.buffer[++currentPosition];
        }else{
            return sample.buffer[--currentPosition];
        }
    }
    bool isAtEnd(Sample &sample){
        return (currentPosition <= 0) || (currentPosition >= sample.size-1);
    }
    void reset(Sample &sample){
        currentPosition = (sample.size-1) / 2;
    }
    private:
    bool direction = true;
};

// play small chunks of the sample
// finds new random position after grain is played
class granularAlgorithm : public playbackAlgorithm{
    public:
    int16_t & play(Sample &sample){
        if(isAtEnd(sample)){
            reset(sample);
        }
        return sample.buffer[grainStartPositions + currentGrainPosition++];
    }
    bool isAtEnd(Sample &sample){
        bool end = (currentGrainPosition >= grainSize);
        if(end){
            reset(sample);
        }
        return end;
    }
    void reset(Sample &sample){
        grainSize = sample.size / 8;
        if(sample.size > grainSize){
            grainStartPositions = random(0, sample.size - grainSize);
        } else {
            grainStartPositions = 0;
        }
        currentGrainPosition = 0;
    }
        
    private:
    unsigned int currentGrainPosition = 0;
    unsigned int grainStartPositions = 0;
    unsigned int grainSize = 16;
};

