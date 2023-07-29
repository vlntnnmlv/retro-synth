#pragma once

#include <SDL2/SDL.h>

enum class OscillatorType
{
    SINUS       = 0,
    SQUARE      = 1,
    TRIANGLE    = 2,
    SAW_ANALOG  = 3,
    SAW_DIGITAL = 4,
    NOISE       = 5
};

class Oscillator
{
    public:
        float oscillate(float frequency, float time, OscillatorType type = OscillatorType::SINUS) const;

    private:
        float F2AV(float frequency) const;
        float sinus(float frequency, float time) const;
        float square(float frequency, float time) const;
        float triangle(float frequency, float time) const;
        float saw_analog(float frequency, float time) const;
        float saw_digital(float frequency, float time) const;
        float noise(float frequency, float time) const;
};