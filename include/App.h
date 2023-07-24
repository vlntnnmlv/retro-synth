#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <cmath>
#include <set>

#include "AudioEngine.h"

class App
{
    public:
        App();
        void start();

    private:
        ~App();

        int init();
        int init_audio_device();
        int init_window();

        void  loop();

        SDL_AudioDeviceID        m_audio_device_id;
        SDL_Window*              m_window;
        SDL_Event                m_current_event;
        SDL_Keycode              m_current_key;
        int                      m_window_width;
        int                      m_window_height;
        bool                     m_running;
        AudioEngine              m_audio_engine;
        std::set<SDL_Keycode>    m_pressed_keys;
};