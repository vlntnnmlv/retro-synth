#pragma once

#include <list>

#include "Oscillator.h"
#include "Envelope.h"

class Instrument
{
    public:
        Instrument()
        {
            m_oscillators = std::list<std::pair<float, OscillatorType>>();
        }

        Instrument(const std::list<std::pair<float, OscillatorType>> &&oscilators)
        {
            m_oscillators = oscilators;
        }

        float get(Note note, float time)
        {
            float result = 0.0f;
            for (auto osc : m_oscillators)
            {
                result += osc.first * m_oscillator.oscillate(note.get_frequency(), time, osc.second);
            }
            return result;
        }

    private:
        Oscillator                m_oscillator;
        std::list<std::pair<float, OscillatorType>> m_oscillators;

};