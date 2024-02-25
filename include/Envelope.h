#pragma once

#include "Note.h"

struct EnvelopeADSR
{
    float attackPeriod;
    float decayPeriod;
    float releasePeriod;

    float attackAmplitude;
    float sustainAmplitude;

    EnvelopeADSR();

    float get_amplitude(Note note, float time) const;

    private:
        float get_amplitude_for_pressed_key(Note note, float time) const;
};