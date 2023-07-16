#pragma once

#include <SDL2/SDL.h>
#include <vector>

class AudioCallbackObject
{
    public:
        static void forward_callback(void* userdata, uint8_t* stream, int len);
        void on_callback(uint8_t* stream, int len);
        void update_frequency(float frequency);

    private:
        uint64_t m_samples_played = 0;
        float m_frequency;
};