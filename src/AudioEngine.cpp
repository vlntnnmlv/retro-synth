#include <iostream>
#include <algorithm>

#include "AudioEngine.h"

AudioEngine::AudioEngine()
{
    m_current_notes = std::list<Note>();

    m_average_amplitude = 0;

    m_octave = 4;
    m_envelope = EnvelopeADSR();
    m_instrument = Instrument(std::list<std::pair<float, OscillatorType>>
        {
            std::pair<float, OscillatorType> { 0.3f, OscillatorType::SAW_ANALOG }
        }
    );
}

AudioEngine::~AudioEngine()
{
    // TODO
}

float AudioEngine::get_audio_time()
{
    return m_audio_time;
}

void AudioEngine::callback(void *userdata, uint8_t* stream, int len)
{
    static_cast<AudioEngine*>(userdata)->on_callback(stream, len);
}

float AudioEngine::get_amplitude()
{
    return m_average_amplitude;
}

int AudioEngine::get_octave()
{
    return m_octave;
}

std::pair<int, const float*> AudioEngine::get_sound_data()
{
    return m_sound_data;
}


void AudioEngine::on_callback(uint8_t* stream, int len)
{
    float *fstream = (float*)(stream);
    int sid = 0;
    for (; sid < (len / 8); ++sid)
    {
        m_audio_time = (m_samples_played + sid) / 44100.0;

        float sound = 0.0f;

        int notes_quantity = m_current_notes.size();
        float modifier = 1.0f / notes_quantity;

        float amplitude = 0.0f;
        float current_freq = 0.0f;
        float average_amplitude = 0.0f;
        
        for (auto note = m_current_notes.begin(); note != m_current_notes.end(); ++note)
        {
            amplitude = m_envelope.get_amplitude(*note, m_audio_time);
            if (note->active && note->time_off > note->time_on && amplitude <= 0.0f)
            {
                note->active = false;
                std::cout << "[T: " << m_audio_time << "] Note set inactive: " << note->type << "\n";
                continue;
            }
            average_amplitude += amplitude;

            current_freq = m_instrument.get(*note, m_audio_time);
            sound += amplitude * current_freq;
        }

        if (notes_quantity != 0)
            average_amplitude /= (float)notes_quantity;
        else
            average_amplitude = 0;
        
        m_average_amplitude = average_amplitude;

        fstream[2 * sid + 0] = sound; /* Left  channel */
        fstream[2 * sid + 1] = sound; /* Right channel */

    }

    // TODO: Find a better way to repsent wave data for shader
    m_sound_data = std::pair<int, const float *>(sid, fstream);
    
    m_samples_played += (len / 8);

    auto note = m_current_notes.begin();
    while (note != m_current_notes.end())
    {
        if (!note->active)
        {
            std::cout << "[T: " << m_audio_time << "] Note deleted: " << note->type << "\n";
            note = m_current_notes.erase(note);
        }
        else
        {
            ++note;
        }
    }
}

void AudioEngine::update_input(std::list<Note> notes_pressed)
{
    // release notes which are not in the input
    for (auto note = m_current_notes.begin(); note != m_current_notes.end(); ++note)
    {
        auto it = std::find(notes_pressed.begin(), notes_pressed.end(), *note);
        if (it == notes_pressed.end() && note->time_off == 0.0f)
        {
            note->time_off = m_audio_time;
            std::cout << "[T: " << m_audio_time << "] Note released: " << note->type << "\n";
        }
    }

    // add notes which are new in input
    for (auto note = notes_pressed.begin(); note != notes_pressed.end(); ++note)
    {
        auto changed_note = std::find(m_current_notes.begin(), m_current_notes.end(), *note);
        if (changed_note == m_current_notes.end())
        {
            note->time_on = m_audio_time;
            note->active = true;
            m_current_notes.push_back(*note);
            std::cout << "[T: " << m_audio_time << "] Note added: " << note->type << "\n";
        }
        else if(changed_note->time_off > changed_note->time_on)
        {
            changed_note->time_off = 0.0f;
            changed_note->time_on = m_audio_time;
            changed_note->active = true;
            std::cout << "[T: " << m_audio_time << "] Note was releasing, but pressed again: " << changed_note->type << "\n";
        }
    }
}

void AudioEngine::increase_octave() { m_octave += 1; }

void AudioEngine::decrease_octave() { m_octave -= 1; }