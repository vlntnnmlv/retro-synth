#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include <cmath>
#include <set>

#include <GL/glew.h>
#include <GL/glu.h>

#include "AudioEngine.h"
#include "ShaderUnit.h"

#include "RtMidi.h"

class App
{
    public:
        App();
        ~App();

        void start();
        int init(AudioEngine *audio_engine);

    private:
        int init_audio();
        int init_window();
        int init_video();

        void loop();
        void poll_event();
        void render();

        bool try_add_input_note(Note note);
        bool try_remove_input_note(Note note);

        Note key2note(SDL_Keycode key);
        Note key2note(std::vector<unsigned char> midi_key);

        bool                     m_running;
        SDL_Window*              m_window;
        int                      m_window_width;
        int                      m_window_height;
        SDL_Event                m_current_event;

        AudioEngine              *m_audio_engine;
        ShaderUnit               *m_shader_unit;
        SDL_AudioDeviceID        m_audio_device_id;

        std::vector<SDL_Keycode> m_possible_keys;
        std::list<Note>          m_pressed_notes;
        SDL_GLContext            m_gl_context;

        RtMidiIn                 m_MIDI_in;

        float                    m_delta_time;
        float                    m_time;

        GLuint                   m_gl_tex;
};