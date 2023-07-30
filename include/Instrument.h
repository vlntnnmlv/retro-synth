#pragma once

#include <list>

#include "Oscillator.h"
#include "Envelope.h"

class Instrument
{
    public:
        Instrument();
        Instrument(const std::list<std::pair<float, OscillatorType>> &&oscilators);

        float get(Note note, float time);

    private:
        Oscillator                                  m_oscillator;
        std::list<std::pair<float, OscillatorType>> m_oscillators;

};