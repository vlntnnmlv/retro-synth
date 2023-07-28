#include <iostream>

#include "AudioEngine.h"

AudioEngine::AudioEngine()
{
    /*
        | a# | b# |        | d# | e# | f# |
    |  a  |  b  |  c  |  d  |  e  |  f  |  g  |
    */

    m_possible_keys = std::vector<SDL_Keycode>
    {
        SDLK_a,
        SDLK_w,
        SDLK_s,
        SDLK_e,
        SDLK_d,
        SDLK_f,
        SDLK_t,
        SDLK_g,
        SDLK_y,
        SDLK_h,
        SDLK_u,
        SDLK_j,
        SDLK_k
    };

    m_current_notes = std::set<Note>();

    m_average_amplitude = 0;

    m_octave = 4;
    m_envelope = EnvelopeADSR();
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

void AudioEngine::on_callback(uint8_t* stream, int len)
{
    float *fstream = (float*)(stream);

    for (int sid = 0; sid < (len / 8); ++sid)
    {
        m_audio_time = (m_samples_played + sid) / 44100.0;

        float sound = 0.0f;

        int notes_quantity = m_current_notes.size();
        float modifier = 1.0f / notes_quantity;

        float amplitude = 0.0f;
        float average_amplitude = 0.0f;
        
        for (auto note = m_current_notes.begin(); note != m_current_notes.end(); ++note)
        {
            amplitude = m_envelope.get_amplitude(*note, m_audio_time);
            if (note->time_off > note->time_on && amplitude <= 0.0f)
                note->active = false;

            average_amplitude += amplitude;
            sound += amplitude * m_oscillator.oscillate(note->get_frequency(), m_audio_time, OscilatorType::TRIANGLE);
        }

        if (notes_quantity != 0)
            average_amplitude /= (float)notes_quantity;
        else
            average_amplitude = 0;
        
        m_average_amplitude = average_amplitude;

        fstream[2 * sid + 0] = sound; /* L */
        fstream[2 * sid + 1] = sound; /* R */
    }

    m_samples_played += (len / 8);

    auto note = m_current_notes.begin();
    while (note != m_current_notes.end())
    {
        if (!note->active)
        {
            note = m_current_notes.erase(note);
        }
        else
        {
            ++note;
        }
    }
}

void AudioEngine::update_input(std::set<SDL_Keycode> keys_pressed)
{
    // convert key codes into notes
    std::set<Note> notes_pressed = std::set<Note>();
    for (SDL_Keycode key : keys_pressed)
    {
        Note note = key2note(key);
        if (note.type != UNKNOWN)
        {
            notes_pressed.insert(note);
            std::cout << note.type << ", ";
        }
    }
    std::cout << "end\n";

    // release notes which are not in the input
    for (auto note = m_current_notes.begin(); note != m_current_notes.end(); ++note)
    {
        if (!notes_pressed.contains(*note) && note->time_off == 0.0f)
        {
            std::cout << "Note released: " << note->type << "\n";
            note->time_off = m_audio_time;
        }
    }

    // add notes which are new in input
    for (Note note : notes_pressed)
    {
        bool isNotePlayed = m_current_notes.contains(note);
        if (!isNotePlayed)
        {
            note.time_on = m_audio_time;
            note.active = true;
            m_current_notes.insert(note);
        }
        else
        {
            auto playing_note = m_current_notes.find(note);
            playing_note->time_off = 0.0f;
            playing_note->time_on = m_audio_time;
            playing_note->active = true;
        }
    } 
}

void AudioEngine::increase_octave() { m_octave += 1; }

void AudioEngine::decrease_octave() { m_octave -= 1; }

Note AudioEngine::key2note(SDL_Keycode key)
{
    int index = -1;
    auto it = std::find(m_possible_keys.begin(), m_possible_keys.end(), key);
    if (it != m_possible_keys.end())
    {
        index = it - m_possible_keys.begin();
    }

    Note note;
    if (index != -1)
    {
        note.type = (NoteType)index;
        note.octave = m_octave;
    }

    return note;
}