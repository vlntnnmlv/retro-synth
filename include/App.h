#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <cmath>
#include <thread>
#include <functional>

#include "AudioCallbackObject.h"

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

        float get_frequency(SDL_Keycode keycode);

        SDL_AudioDeviceID m_audio_device_id;
        int m_window_width;
        int m_window_height;
        SDL_Window* m_window;
        bool m_running;
        SDL_Keycode m_current_key;
        std::vector<SDL_Keycode> m_keys;
        AudioCallbackObject m_audio_callback_object;
};