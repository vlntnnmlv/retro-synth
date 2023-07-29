#pragma once

#include <list>

#include "Oscillator.h"
#include "Envelope.h"

class Instrument
{
    public:
        Instrument(const std::list<OscillatorType> &&oscilators)
        {
            m_oscillators = oscilators;
        }

        float get(Note note, float time)
        {
            float result = 0.0f;
            for (auto osc : m_oscillators)
            {
                result += m_oscillator.oscillate(note.get_frequency(), time, osc);
            }
            return result;
        }

    private:
        Oscillator                m_oscillator;
        std::list<OscillatorType> m_oscillators;

};