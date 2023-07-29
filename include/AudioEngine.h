#pragma once

#include <SDL2/SDL.h>
#include <cmath>
#include <vector>
#include <list>

#include "Envelope.h"
#include "Oscillator.h"
#include "Note.h"
#include "Instrument.h"

class AudioEngine
{
    public:
        AudioEngine();
        void update_input(std::list<Note> notes_pressed);
        void increase_octave();
        void decrease_octave();
        float get_audio_time();
        float get_amplitude();
        int   get_octave();
        static void callback(void *userdata, uint8_t* stream, int len);

    private:
        void  on_callback(uint8_t* stream, int len);

        EnvelopeADSR m_envelope;
        Oscillator   m_oscillator;
        float        m_frequency;
        float        m_average_amplitude;
        float        m_audio_time;
        int          m_samples_played;

        std::list<Note>           m_current_notes;
        int                       m_octave;
        Instrument                *m_instrument;
};