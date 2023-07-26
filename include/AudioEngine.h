#pragma once

#include <SDL2/SDL.h>
#include <cmath>
#include <vector>
#include <set>

#include "Envelope.h"
#include "Oscillator.h"
#include "Note.h"

class AudioEngine
{
    public:
        AudioEngine();
        void update_input(std::set<SDL_Keycode> keys_pressed);
        void increase_octave();
        void decrease_octave();
        float get_audio_time();
        float get_amplitude();
        static void callback(void *userdata, uint8_t* stream, int len);

    private:
        void  on_callback(uint8_t* stream, int len);
        Note  key2note(SDL_Keycode key);

        EnvelopeADSR m_envelope;
        Oscillator   m_oscillator;
        float        m_frequency;
        float        m_average_amplitude;
        float        m_audio_time;
        int          m_samples_played;

        std::vector<SDL_Keycode>  m_possible_keys;
        std::set<Note>            m_current_notes;
        int                       m_octave;
};