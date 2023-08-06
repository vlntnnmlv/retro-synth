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
        AudioEngine(int frequency, int channels, int samples);
        ~AudioEngine();

        void  update_input(std::list<Note> notes_pressed);
        void  increase_octave();
        void  decrease_octave();
        float get_audio_time();
        float get_amplitude();
        int   get_octave();

        std::vector<float> *get_sound_data();
        std::vector<float> *m_sound_data;

        static void callback(void *userdata, uint8_t* stream, int len);

    private:
        void on_callback(uint8_t* stream, int len);

        SDL_AudioDeviceID m_audio_device_id;
        float             m_frequency;
        float             m_average_amplitude;
        float             m_audio_time;
        int               m_samples_played;

        int               m_octave;
        EnvelopeADSR      m_envelope;
        Instrument        m_instrument;
        std::list<Note>   m_current_notes;
};