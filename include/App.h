#pragma once

#include <SDL2/SDL.h>
#include <cmath>
#include <vector>
#include <unordered_map>


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
        SDL_Window* m_window;
        int m_window_width;
        int m_window_height;
        bool m_running;
        SDL_Keycode m_current_key;
        std::vector<SDL_Keycode> m_keys;

        std::unordered_map<SDL_Keycode, bool> m_keys_pressed;

        AudioCallbackObject m_audio_callback_object;
};