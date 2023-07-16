#pragma once

#include <SDL2/SDL.h>
#include <vector>

struct AudioCallbackObject
{
    public:
        void OnCallback(uint8_t* stream, int len)
        {
            float* fstream = (float*)(stream);
            static const float volume = 0.2;

            for (int sid = 0; sid < (len / 8); ++sid)
            {
                double time = (m_samples_played + sid) / 44100.0;
                fstream[2 * sid + 0] = (volume * sin(m_frequency * 2.0 * M_PI * time)); /* L */
                fstream[2 * sid + 1] = (volume * sin(m_frequency * 2.0 * M_PI * time));/* R */
            }

            m_samples_played += (len / 8);
        }

        static void forward_callback(void* userdata, uint8_t* stream, int len)
        {
            static_cast<AudioCallbackObject*>(userdata)->OnCallback(stream, len);
        }

        void update_frequency(float frequency)
        {
            m_frequency = frequency;
        }

    private:
        uint64_t m_samples_played = 0;
        float m_frequency;
};