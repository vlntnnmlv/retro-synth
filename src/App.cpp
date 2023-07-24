#include <iostream>

#include "App.h"
#include <set>

App::App()
{
    m_pressed_keys = std::set<SDL_Keycode>();

    init();
}

App::~App()
{
    SDL_DestroyWindow(m_window);
    SDL_CloseAudioDevice(m_audio_device_id);
    SDL_Quit();
}

int App::init()
{
    // SDL Audio subsystem initialization
    if(SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) < 0)
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

    audio_spec_want.callback = AudioEngine::callback;
    audio_spec_want.userdata = &m_audio_engine;

    m_audio_device_id = SDL_OpenAudioDevice(NULL, 0, &audio_spec_want, &audio_spec, SDL_AUDIO_ALLOW_FORMAT_CHANGE);

    if(!m_audio_device_id)
    {
        fprintf(stderr, "Error creating SDL audio device. SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    SDL_PauseAudioDevice(m_audio_device_id, 0);

    return 0;
}

int App::init_window()
{
    m_window_width = 600;
    m_window_height = 600;
    m_window = SDL_CreateWindow(
            "Retro-Synth",
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
        loop();
    }
}

void App::loop()
{
    while(SDL_PollEvent(&m_current_event) != 0)
    {
        if(m_current_event.type == SDL_QUIT)
            m_running = false;
        if (m_current_event.type == SDL_KEYDOWN)
        {
            if (m_current_event.key.keysym.sym == SDLK_ESCAPE)
                m_running = false;

            if (m_current_event.key.keysym.sym == SDLK_z)
            {
                m_audio_engine.decrease_octave();
            }

            if (m_current_event.key.keysym.sym == SDLK_x)
            {
                m_audio_engine.increase_octave();
            }

            if (!m_pressed_keys.contains(m_current_event.key.keysym.sym))
            {
                m_pressed_keys.insert(m_current_event.key.keysym.sym);
                m_audio_engine.update_input(m_pressed_keys);
            }
        }
        if (m_current_event.type == SDL_KEYUP)
        {
            if (m_pressed_keys.contains(m_current_event.key.keysym.sym))
            {
                m_pressed_keys.erase(m_current_event.key.keysym.sym);
                m_audio_engine.update_input(m_pressed_keys);
            }
        }
    }
}