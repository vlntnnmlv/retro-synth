#pragma once

#include <SDL2/SDL.h>

enum OscilatorType
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
        float operator() (float frequency, float time, OscilatorType type = SINUS)
        {
            switch (type)
            {
                case SINUS:
                    return sinus(frequency, time);
                case SQUARE:
                    return square(frequency, time);
                case TRIANGLE:
                    return triangle(frequency, time);
                case SAW_ANALOG:
                    return saw_analog(frequency, time);
                case SAW_DIGITAL:
                    return saw_digital(frequency, time);
                case NOISE:
                    return noise(frequency, time);
            }
        }

    private:
        float F2AV(float frequency)
        {
            return frequency * 2.0 * M_PI;
        }
        
        float sinus(float frequency, float time)
        {
            return sin(F2AV(frequency) * time + 0.01f * frequency * sin(F2AV(5.0f) * time));
        }

        float square(float frequency, float time)
        {
            return sin(F2AV(frequency) * time) > 0 ? 1.0f : -1.0f;
        }

        float triangle(float frequency, float time)
        {
            return asin(sin(F2AV(frequency) * time)) * (2.0f / M_PI);
        }

        float saw_analog(float frequency, float time)
        {
            float result = 0.0f;

            for (float n = 1.0f; n < 40.0f; n += 1.0f)
            {
                result += sin(n * F2AV(frequency) * time) / n;
            }
            return result * (2.0f / M_PI);
        }

        float saw_digital(float frequency, float time)
        {
            return (2.0f / M_PI) * (frequency * M_PI * fmod(time, 1.0f / frequency) - (M_PI / 2.0f));
        }

        float noise(float frequency, float time)
        {
            return 2.0 * ((double)rand() / (double)RAND_MAX) - 1.0;
        }


};