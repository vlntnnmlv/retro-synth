#include <iostream>
#include <fstream>
#include <streambuf>

#include <SDL_image.h>
#include <SDL_ttf.h>

#include "App.h"
#include "Utility.h"

App::App()
{
    m_pressed_notes = std::list<Note>();

    /*
        | c# | d# |        | f# | g# | a# |
    |  c  |  d  |  e  |  f  |  g  |  a  |  b  |
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

    m_delta_time = 0;
    m_time = 0;

    init();
}

App::~App()
{
    // Clean shaders
    SDL_GL_DeleteContext(m_gl_context);

    // Everything else
    SDL_DestroyWindow(m_window);
    SDL_CloseAudioDevice(m_audio_device_id);
    SDL_Quit();
}

int App::init()
{
    // SDL subsystems initialization
    if(SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "Error initializing SDL. SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Find and initialize audio engine
    init_audio();

    // Create application window
    init_window();
    init_video();

    return 0;
}

int App::init_audio()
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

int App::init_video()
{
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    m_gl_context = SDL_GL_CreateContext(m_window);

    if (m_gl_context == NULL)
    {
        fprintf(stderr, "OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
        return 1;
    }

    // Use VSYNC
    if(SDL_GL_SetSwapInterval(1) < 0)
    {
        printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
    }

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    if(glewError != GLEW_OK)
    {
        printf( "Error initializing GLEW! %s\n", glewGetErrorString( glewError ) );
    }

    // Initialize Shaders
    m_shader_unit = new ShaderUnit(m_window_width, m_window_height);

    return 0;
}

void App::render()
{
    std::pair<int, const float*> sound_data = m_audio_engine.get_sound_data();
    m_shader_unit->m_setter.set_1f("in_Time", m_audio_engine.get_audio_time());
    m_shader_unit->m_setter.set_1f("in_Amplitude", m_audio_engine.get_amplitude());

    m_shader_unit->m_setter.set_1f("in_SoundDataCount", sound_data.first);
    m_shader_unit->m_setter.set_1fv("in_SoundData", sound_data.first, sound_data.second);

    // Redraw everything
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
    SDL_GL_SwapWindow(m_window);
}

void App::start()
{
    m_running = true;
    unsigned int nPorts = m_MIDI_in.getPortCount();
    if ( nPorts != 0 )
    {
        m_MIDI_in.openPort(0);
        m_MIDI_in.ignoreTypes(false, false, false);
    }

    while (m_running)
    {
        loop();
    }
}

void App::loop()
{
    Uint64 frame_start_time = SDL_GetPerformanceCounter();
    
    while(SDL_PollEvent(&m_current_event) != 0)
    {
        poll_event();
    }

    std::vector<unsigned char> message;
    int nBytes, i;
    double stamp;

    stamp = m_MIDI_in.getMessage(&message);
    nBytes = message.size();
    if (nBytes >= 2)
    {
        if (message[0] == 156)
        {
            try_add_input_note(midi_key2note(message));
        }
        if (message[0] == 140)
        {
            try_remove_input_note(midi_key2note(message));
        }
    }

    render();

    Uint64 frame_end_time = SDL_GetPerformanceCounter();
    m_delta_time = (frame_end_time - frame_start_time) / (float)SDL_GetPerformanceFrequency();
    m_time += m_delta_time;
}

Note App::key2note(SDL_Keycode key)
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
        note.octave = m_audio_engine.get_octave();
    }

    return note;
}

Note App::midi_key2note(std::vector<unsigned char> midi_key)
{
    Note note;
    std::cout << "Note id " << (int)midi_key[1] << "\n";
    note.type = (NoteType)((int)midi_key[1] % 12);
    std::cout << "Note type " << note.octave << "\n";
    note.octave = (int)(midi_key[1] / 12);
    std::cout << "Note octave " << note.octave << "\n";
    return note;
}


void App::poll_event()
{
    if(m_current_event.type == SDL_QUIT)
        m_running = false;
    if (m_current_event.type == SDL_WINDOWEVENT && m_current_event.window.event == SDL_WINDOWEVENT_RESIZED)
    {
        SDL_Window* win = SDL_GetWindowFromID(m_current_event.window.windowID);
        if (win == m_window)
        {
            m_window_width = m_current_event.window.data1;
            m_window_height = m_current_event.window.data2;
        }
    }

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

        auto it = std::find(m_possible_keys.begin(), m_possible_keys.end(), m_current_event.key.keysym.sym);
        if (it != m_possible_keys.end())
        {
            Note note = key2note(m_current_event.key.keysym.sym);
            try_add_input_note(note);
        }
    }
    if (m_current_event.type == SDL_KEYUP)
    {
        auto it = std::find(m_possible_keys.begin(), m_possible_keys.end(), m_current_event.key.keysym.sym);
        if (it != m_possible_keys.end())
        {
            Note note = key2note(m_current_event.key.keysym.sym);
            try_remove_input_note(note);
        }
    }   
}

bool App::try_add_input_note(Note note)
{
    auto it = std::find(m_pressed_notes.begin(), m_pressed_notes.end(), note);
    if (it == m_pressed_notes.end())
    {
        m_pressed_notes.push_back(note);
        m_audio_engine.update_input(m_pressed_notes);
        return true;
    }
    return false;
}

bool App::try_remove_input_note(Note note)
{
    auto it = std::find(m_pressed_notes.begin(), m_pressed_notes.end(), note);
    if (it != m_pressed_notes.end())
    {
        m_pressed_notes.erase(it);
        m_audio_engine.update_input(m_pressed_notes);
        return true;
    }
    return false;
}