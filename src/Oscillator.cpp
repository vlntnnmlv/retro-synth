#include "Oscillator.h"

float Oscillator::oscillate(float frequency, float time, OscilatorType type) const
{
    switch (type)
    {
        case OscilatorType::SINUS:
            return sinus(frequency, time);
        case OscilatorType::SQUARE:
            return square(frequency, time);
        case OscilatorType::TRIANGLE:
            return triangle(frequency, time);
        case OscilatorType::SAW_ANALOG:
            return saw_analog(frequency, time);
        case OscilatorType::SAW_DIGITAL:
            return saw_digital(frequency, time);
        case OscilatorType::NOISE:
            return noise(frequency, time);
    }
}

float Oscillator::F2AV(float frequency) const
{
    return frequency * 2.0 * M_PI;
}

float Oscillator::sinus(float frequency, float time) const
{
    return sin(F2AV(frequency) * time + 0.01f * frequency * sin(F2AV(5.0f) * time));
}

float Oscillator::square(float frequency, float time) const
{
    return sin(F2AV(frequency) * time) > 0 ? 1.0f : -1.0f;
}

float Oscillator::triangle(float frequency, float time) const
{
    return asin(sin(F2AV(frequency) * time)) * (2.0f / M_PI);
}

float Oscillator::saw_analog(float frequency, float time) const
{
    float result = 0.0f;

    for (float n = 1.0f; n < 40.0f; n += 1.0f)
    {
        result += sin(n * F2AV(frequency) * time) / n;
    }
    return result * (2.0f / M_PI);
}

float Oscillator::saw_digital(float frequency, float time) const
{
    return (2.0f / M_PI) * (frequency * M_PI * fmod(time, 1.0f / frequency) - (M_PI / 2.0f));
}

float Oscillator::noise(float frequency, float time) const
{
    return 2.0 * ((double)rand() / (double)RAND_MAX) - 1.0;
}