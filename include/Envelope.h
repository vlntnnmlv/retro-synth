#pragma once

#include "Note.h"

struct EnvelopeADSR
{
    float attackPeriod;
    float decayPeriod;
    float releasePeriod;

    float attackAmplitude;
    float sustainAmplitude;

    EnvelopeADSR()
    {
        attackPeriod = 0.02f;
        decayPeriod = 0.02f;
        releasePeriod = 1.0f;

        attackAmplitude = 1.0f;
        sustainAmplitude = 0.6f;
    }

    float get_amplitude(Note note, float time)
    {
        float amplitude = 0.0f;

        if (note.timeOn > note.timeOff)
        {
            // ADS
            amplitude = get_amplitude_for_pressed_key(note, time);
        }
        else
        {
            // R
            amplitude = ((time - note.timeOff) / releasePeriod) * (0.0f - sustainAmplitude) + get_amplitude_for_pressed_key(note, time);
        }

        if (amplitude <= 0.001f)
        {
            amplitude = 0.0f;
        }

        return amplitude;
    }

    private:
        float get_amplitude_for_pressed_key(Note note, float time)
        {
            float lifeTime = time - note.timeOn;

            // Attack
            if (lifeTime <= attackPeriod)
            {
                return (lifeTime / attackPeriod) * attackAmplitude;// + get_amplitude_for_released_key(note, time);
            }

            // Decay
            if (lifeTime > attackPeriod && lifeTime <= attackPeriod + decayPeriod)
            {
                return ((lifeTime - attackPeriod) / decayPeriod) * (sustainAmplitude - attackAmplitude) + attackAmplitude;
            }

            // Sustain
            if (lifeTime > attackPeriod + decayPeriod)
            {
                return sustainAmplitude;
            }

            return 0.0f;
        }

        float get_amplitude_for_released_key(Note note, float time)
        {
            return ((time - note.timeOff) / releasePeriod) * (0.0f - sustainAmplitude) + get_amplitude_for_pressed_key(note, time);
        }
};