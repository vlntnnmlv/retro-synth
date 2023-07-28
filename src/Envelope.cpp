#include "Envelope.h"

EnvelopeADSR::EnvelopeADSR()
{
    attackPeriod = 5.0f;
    decayPeriod = 0.5f;
    releasePeriod = 1.0f;

    attackAmplitude = 1.0f;
    sustainAmplitude = 0.6f;
}

float EnvelopeADSR::get_amplitude(Note note, float time) const
{
    float amplitude = 0.0f;

    if (note.time_on > note.time_off)
    {
        // ADS
        amplitude = get_amplitude_for_pressed_key(note, time);
    }
    else
    {
        // R
        amplitude = ((time - note.time_off) / releasePeriod) * (0.0f - sustainAmplitude) + get_amplitude_for_pressed_key(note, time);
    }

    if (amplitude <= 0.001f)
    {
        amplitude = 0.0f;
    }

    return amplitude;
}

float EnvelopeADSR::get_amplitude_for_pressed_key(Note note, float time) const
{
    float lifeTime = time - note.time_on;

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

float EnvelopeADSR::get_amplitude_for_released_key(Note note, float time) const
{
    return ((time - note.time_off) / releasePeriod) * (0.0f - sustainAmplitude) + get_amplitude_for_pressed_key(note, time);
}