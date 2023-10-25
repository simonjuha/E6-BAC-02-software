#include <vector>
#include <string>
#include "sdCardDriver/sample.hpp"


// ISdCardLoad interface
class ISdCardLoad
{
    public:
    virtual int getSampleByName(std::string fileName, Sample *sample) = 0;
};

// ISdCardObserver interface
class ISdCardObserver
{
    public:
        virtual void update() = 0;
};

// ISdCardInfo interface
class ISdCardInfo{
    virtual int getNumberOfSamples() = 0;
    virtual std::vector<std::string> getSampleNames() = 0;
};
