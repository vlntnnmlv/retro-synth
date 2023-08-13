#include <iostream>
#include <algorithm>

#include "AudioEngine.h"

AudioEngine::AudioEngine(int frequency, int channels, int samples):
m_frequency(frequency),
m_channels(channels),
m_samples(samples)
{

}

AudioEngine::~AudioEngine()
{
    SDL_CloseAudioDevice(m_audio_device_id);
}

void AudioEngine::init()
{
    SDL_AudioSpec audio_spec_want, audio_spec;
    SDL_memset(&audio_spec_want, 0, sizeof(audio_spec_want));

    audio_spec_want.freq     = m_frequency;
    audio_spec_want.format   = AUDIO_F32;
    audio_spec_want.channels = m_channels;
    audio_spec_want.samples  = m_samples;

    audio_spec_want.callback = AudioEngine::callback;
    audio_spec_want.userdata = this;

    m_audio_device_id = SDL_OpenAudioDevice(NULL, 0, &audio_spec_want, &audio_spec, SDL_AUDIO_ALLOW_FORMAT_CHANGE);

    if(!m_audio_device_id)
    {
        fprintf(stderr, "Error creating SDL audio device. SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
    }

    SDL_PauseAudioDevice(m_audio_device_id, 0);

    m_current_notes = std::list<Note>();

    m_average_amplitude = 0;

    m_octave = 4;
    m_envelope = EnvelopeADSR();
    m_instrument = Instrument(std::list<std::pair<float, OscillatorType>>
        {
            std::pair<float, OscillatorType> { 0.3f, OscillatorType::SAW_ANALOG }
        }
    );

    m_sound_data = new std::vector<float>(512);
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

std::vector<float> *AudioEngine::get_sound_data()
{
    return m_sound_data;
}

void AudioEngine::on_callback(uint8_t* stream, int len)
{
    float *fstream = (float*)(stream);
    std::vector<float> sound_data = std::vector<float>();

    for (int sid = 0; sid < (len / 8); ++sid)
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

        sound_data.push_back(sound / 100.0);
    }

    // std::cout << "Before clean: " << m_sound_data->size() << "\n";
    m_sound_data->clear();
    // std::cout << "After clean: " << m_sound_data->size() << "\n";
    // std::cout << "Tmp        : " << sound_data.size() << "\n";
    m_sound_data->insert(m_sound_data->begin(), sound_data.begin(), sound_data.end());
    // std::cout << "After insert: " << m_sound_data->size() << "\n";

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