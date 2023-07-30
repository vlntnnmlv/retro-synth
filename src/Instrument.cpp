#include "Instrument.h"

Instrument::Instrument()
{
    m_oscillators = std::list<std::pair<float, OscillatorType>>();
}

Instrument::Instrument(const std::list<std::pair<float, OscillatorType>> &&oscilators)
{
    m_oscillators = oscilators;
}

float Instrument::get(Note note, float time)
{
    float result = 0.0f;
    for (auto osc : m_oscillators)
    {
        result += osc.first * m_oscillator.oscillate(note.get_frequency(), time, osc.second);
    }
    return result;
}