#include <Arduino.h>
#include <SPI.h>
#include "sdCardDriver/sample.hpp"

// playbackAlgorithm interface
class playbackAlgorithm
{
    public:
    virtual int16_t & play(Sample &sample) = 0; // returns the next sample
    virtual bool isAtEnd(Sample &sample) = 0;   // returns true if the end of the sample is reached
    virtual void reset(Sample &sample) = 0;     // resets the algorithm to the beginning of the sample
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
        return (currentPosition >= sample.size-1); // end of sample
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
        if(isAtEnd(sample)){                        // is at beginning of sample
            currentPosition = sample.size-1;        // set position to end of sample
        }
        return sample.buffer[--currentPosition];    // return sample at current position and decrement position
    }
    bool isAtEnd(Sample &sample){
        return (currentPosition <= 0);  // beginning of sample
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
        if(currentPosition >= sample.size-1){ // if at end of sample
            currentPosition = sample.size-1;  
            direction = false; // change direction
        }
        if(currentPosition <= 0){ // if at beginning of sample
            currentPosition = 0;
            direction = true; // change direction
        }
        if(direction){
            return sample.buffer[++currentPosition]; // forward
        }else{
            return sample.buffer[--currentPosition]; // backward
        }
    }
    // isAtEnd: is not use internally, but used by the audioDrier when in TRIGGER_ONCE_MODE 
    bool isAtEnd(Sample &sample){
        return (currentPosition <= 0) || (currentPosition >= sample.size-1); // beginning or end of sample
    }
    void reset(Sample &sample){
        currentPosition = (sample.size-1) / 2; // set position to middle of sample on reset
    }
    private:
    bool direction = true;
};

// play small chunks of the sample
// finds new random position after grain is played (forward)
class granularAlgorithm : public playbackAlgorithm{
    public:
    int16_t & play(Sample &sample){
        if(isAtEnd(sample)){
            reset(sample);
        }
        return sample.buffer[grainStartPositions + currentGrainPosition++];
    }
    // isAtEnd: in this algorith isAtEnd is used to find the end of the grain and not the whole sample
    bool isAtEnd(Sample &sample){
        bool end = (currentGrainPosition >= grainSize); // end of grain
        if(end){
            reset(sample);
        }
        return end;
    }
    void reset(Sample &sample){
        grainSize = sample.size / 8; // grainsize is currently 1/8 of the sample size
        if(sample.size > grainSize){
            grainStartPositions = random(0, sample.size - grainSize); // get random start position
        } else {
            grainStartPositions = 0; // if sample is smaller than grainsize set start position to 0
        }
        currentGrainPosition = 0;
    }
        
    private:
    unsigned int currentGrainPosition = 0;
    unsigned int grainStartPositions = 0;
    unsigned int grainSize = 16;
};

