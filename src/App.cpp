#include <iostream>

#include "App.h"

App::App()
{
    m_keys = std::vector<SDL_Keycode> {
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

    init();
}

App::~App()
{
    // SDL_DestroyWindow(window);
    SDL_CloseAudioDevice(m_audio_device_id);
    SDL_Quit();
}

int App::init()
{
    // SDL Audio subsystem initialization
    if(SDL_Init(SDL_INIT_AUDIO) < 0)
    {
        fprintf(stderr, "Error initializing SDL. SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    // Find and initialize audio device
    init_audio_device();

    // Create application window
    init_window();

    return 0;
}

int App::init_audio_device()
{
    SDL_AudioSpec audio_spec_want, audio_spec;
    SDL_memset(&audio_spec_want, 0, sizeof(audio_spec_want));

    audio_spec_want.freq     = 44100;
    audio_spec_want.format   = AUDIO_F32;
    audio_spec_want.channels = 2;
    audio_spec_want.samples  = 512;

    audio_spec_want.callback = AudioCallbackObject::forward_callback;
    audio_spec_want.userdata = &m_audio_callback_object;

    m_audio_device_id = SDL_OpenAudioDevice(
        NULL, 0,
        &audio_spec_want, &audio_spec,
        SDL_AUDIO_ALLOW_FORMAT_CHANGE
    );

    if(!m_audio_device_id)
    {
        fprintf(stderr, "Error creating SDL audio device. SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    return 0;
}

int App::init_window()
{
    m_window_width = 600;
    m_window_height = 600;
    m_window = SDL_CreateWindow(
            "SDL Tone Generator",
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            m_window_width, m_window_height,
            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
        );

    if(!m_window)
    {
        fprintf(stderr, "Error creating SDL window. SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    return 0;
}

void App::start()
{
    m_running = true;
    while (m_running)
    {
        SDL_Event sdl_event;
        while(SDL_PollEvent(&sdl_event) != 0)
        {
            if(sdl_event.type == SDL_QUIT)
                m_running = false;
			if (sdl_event.type == SDL_KEYDOWN)
			{
                if (sdl_event.key.keysym.sym == SDLK_ESCAPE)
                    m_running = false;

                m_current_key = sdl_event.key.keysym.sym;
                SDL_PauseAudioDevice(m_audio_device_id, 0);
			}
			if (sdl_event.type == SDL_KEYUP)
			{
                SDL_PauseAudioDevice(m_audio_device_id, 1);
			}
        }
        m_audio_callback_object.update_frequency(get_frequency(m_current_key));
    }
}

float App::get_frequency(SDL_Keycode keycode)
{
    auto it = std::find(m_keys.begin(), m_keys.end(), keycode);
    int index = -1;

    if (it != m_keys.end())
        index = it - m_keys.begin();
    
    if (index == -1)
        return 0;

    return 440.0f * std::pow(2, index / 12.0f);
}