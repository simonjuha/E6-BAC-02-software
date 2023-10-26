#ifndef SAMPLE_HPP
#define SAMPLE_HPP

#include <cstdint> // int16_t
#include <cstddef> // size_t

struct Sample{
    // Default constructor
    Sample() : buffer(nullptr), size(0) {}

    // Parameterized constructor
    Sample(size_t newSize, int16_t* newBuffer) : size(newSize) {
        buffer = new int16_t[size];
        if (newBuffer) {
            // Copy data from newBuffer to buffer
            for (size_t i = 0; i < size; ++i) {
                buffer[i] = newBuffer[i];
            }
        }
    }
    
    // Destructor
    ~Sample() {
        delete[] buffer;
    }

    int16_t *buffer;
    size_t size;
    // sample name?
};

#endif // SAMPLE_HPP