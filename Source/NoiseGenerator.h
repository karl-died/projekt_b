/*
  ==============================================================================

    NoiseGenerator.h
    Created: 7 Jul 2025 9:21:27pm
    Author:  Karl Diedrichsen

  ==============================================================================
*/

#pragma once

class NoiseGenerator
{
public:
    NoiseGenerator() {}
    ~NoiseGenerator() {}
    
    float getNextValue()
    {
        //xorshift based random number generator
        x ^= x << 13;
        x ^= x >> 17;
        x ^= x << 5;
        
        const uint32_t div = -1;
        return ((float) x / div) - 0.5;
    }
    
    void store() { tx = x; }
    void recall() { x = tx; }
    
private:
    uint32_t x = 1;
    uint32_t tx = x;
};