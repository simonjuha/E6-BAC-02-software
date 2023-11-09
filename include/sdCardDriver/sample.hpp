#ifndef SAMPLE_HPP
#define SAMPLE_HPP

#include <cstdint> // int16_t
#include <cstddef> // size_t

struct Sample{
    // Default constructor
    Sample() : size(0) {}

    // Parameterized constructor
    Sample(size_t newSize, const std::vector<int16_t>& newBuffer) : 
        size(newSize), 
        buffer(newBuffer) 
        {}

    // empty sample
    Sample(size_t newSize) : size(newSize) {
        buffer.resize(size);
    }
    
    std::vector<int16_t> buffer;
    size_t size;
    // sample name?
};

#endif // SAMPLE_HPP