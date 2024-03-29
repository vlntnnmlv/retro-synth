// #include <iostream>
// #include <fstream>
// #include <streambuf>

// #include <SDL_image.h>

// #include "App.h"
// #include "Utility.h"

// App::App()
// {
//     m_pressed_notes = std::list<Note>();

//     /*
//         | c# | d# |        | f# | g# | a# |
//     |  c  |  d  |  e  |  f  |  g  |  a  |  b  |
//     */
//     m_possible_keys = std::vector<SDL_Keycode>
//     {
//         SDLK_a,
//         SDLK_w,
//         SDLK_s,
//         SDLK_e,
//         SDLK_d,
//         SDLK_f,
//         SDLK_t,
//         SDLK_g,
//         SDLK_y,
//         SDLK_h,
//         SDLK_u,
//         SDLK_j,
//         SDLK_k
//     };

//     m_delta_time = 0;
//     m_time = 0;
// }

// App::~App()
// {
//     // Delete heap allocated objects
//     std::cout << "App removed\n";
//     delete m_shader_unit;

//     // Clean shaders
//     SDL_GL_DeleteContext(m_gl_context);

//     // Everything else
//     SDL_DestroyWindow(m_window);
//     SDL_Quit();
// }

// int App::init(AudioEngine *audio_engine)
// {
//     // SDL subsystems initialization
//     if(SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) < 0)
//     {
//         fprintf(stderr, "Error initializing SDL. SDL_Error: %s\n", SDL_GetError());
//         return 1;
//     }

//     m_audio_engine = audio_engine;

//     init_audio();
//     init_window();
//     init_video();

//     return 0;
// }

// int App::init_audio()
// {
//     m_audio_engine->init();

//     return 0;
// }

// int App::init_window()
// {
//     m_window_width = 600;
//     m_window_height = 600;
//     m_window = SDL_CreateWindow(
//             "Retro-Synth",
//             SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
//             m_window_width, m_window_height,
//             SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
//         );

//     if(!m_window)
//     {
//         fprintf(stderr, "Error creating SDL window. SDL_Error: %s\n", SDL_GetError());
//         SDL_Quit();
//         return -1;
//     }

//     return 0;
// }

// int App::init_video()
// {
//     SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
//     SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
//     SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
//     SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

//     m_gl_context = SDL_GL_CreateContext(m_window);

//     if (m_gl_context == NULL)
//     {
//         fprintf(stderr, "OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
//         return 1;
//     }

//     // Use VSYNC
//     if(SDL_GL_SetSwapInterval(1) < 0)
//     {
//         printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
//     }

//     // Initialize GLEW
//     glewExperimental = GL_TRUE;
//     GLenum glewError = glewInit();
//     if(glewError != GLEW_OK)
//     {
//         printf( "Error initializing GLEW! %s\n", glewGetErrorString( glewError ) );
//     }

//     // Initialize Shaders
//     m_shader_unit = new ShaderUnit(m_window_width, m_window_height);

//     std::vector<float> sound_data = std::vector<float>(512);
//     glGenTextures(1, &m_gl_tex);
//     // glActiveTexture(GL_TEXTURE0);
//     glBindTexture(GL_TEXTURE_2D, m_gl_tex);
//     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 512, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
//     // glUniform1i(glGetUniformLocation(m_shader_unit->m_gl_program_id, "in_AudioData"), 0);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_BORDER);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_BORDER);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//     // glEnable(GL_BLEND);
//     // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

//     return 0;
// }

// void App::render()
// {
//     ///
//     glActiveTexture(GL_TEXTURE0);
//     glBindTexture(GL_TEXTURE_2D, m_gl_tex);
//     std::vector<float> *sound_data = m_audio_engine->get_sound_data();
//     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 512, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, sound_data->data());
//     // std::cout << sound_data->size() << "\n";
//     glUniform1i(glGetUniformLocation(m_shader_unit->m_gl_program_id, "in_AudioData"), 0);
//     ///

//     m_shader_unit->m_setter.set_1f("in_Time", m_audio_engine->get_audio_time());
//     m_shader_unit->m_setter.set_1f("in_Amplitude", m_audio_engine->get_amplitude());

//     // Redraw everything
//     glClear(GL_COLOR_BUFFER_BIT);
//     glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
//     SDL_GL_SwapWindow(m_window);
// }

// void App::start()
// {
//     m_running = true;
//     unsigned int nPorts = m_MIDI_in.getPortCount();
//     if ( nPorts != 0 )
//     {
//         m_MIDI_in.openPort(0);
//         m_MIDI_in.ignoreTypes(false, false, false);
//     }

//     while (m_running)
//     {
//         loop();
//     }
// }

// void App::loop()
// {
//     Uint64 frame_start_time = SDL_GetPerformanceCounter();
    
//     poll_event();

//     std::vector<unsigned char> message;
//     int nBytes, i;
//     double stamp;

//     stamp = m_MIDI_in.getMessage(&message);
//     nBytes = message.size();
//     if (nBytes >= 2)
//     {
//         if (message[0] == 156)
//         {
//             try_add_input_note(key2note(message));
//         }
//         if (message[0] == 140)
//         {
//             try_remove_input_note(key2note(message));
//         }
//     }

//     render();

//     Uint64 frame_end_time = SDL_GetPerformanceCounter();
//     m_delta_time = (frame_end_time - frame_start_time) / (float)SDL_GetPerformanceFrequency();
//     m_time += m_delta_time;
// }

// Note App::key2note(SDL_Keycode key)
// {
//     int index = -1;
//     auto it = std::find(m_possible_keys.begin(), m_possible_keys.end(), key);
//     if (it != m_possible_keys.end())
//     {
//         index = it - m_possible_keys.begin();
//     }

//     Note note;
//     if (index != -1)
//     {
//         note.type = (NoteType)index;
//         note.octave = m_audio_engine->get_octave();
//     }

//     return note;
// }

// Note App::key2note(std::vector<unsigned char> midi_key)
// {
//     Note note;
//     note.type = (NoteType)((int)midi_key[1] % 12);
//     note.octave = (int)(midi_key[1] / 12);
//     return note;
// }

// void App::poll_event()
// {
//     while(SDL_PollEvent(&m_current_event) != 0)
//     {
//         if(m_current_event.type == SDL_QUIT)
//             m_running = false;
//         if (m_current_event.type == SDL_WINDOWEVENT && m_current_event.window.event == SDL_WINDOWEVENT_RESIZED)
//         {
//             SDL_Window* win = SDL_GetWindowFromID(m_current_event.window.windowID);
//             if (win == m_window)
//             {
//                 m_window_width = m_current_event.window.data1;
//                 m_window_height = m_current_event.window.data2;
//             }
//         }

//         if (m_current_event.type == SDL_KEYDOWN)
//         {
//             if (m_current_event.key.keysym.sym == SDLK_ESCAPE)
//                 m_running = false;

//             if (m_current_event.key.keysym.sym == SDLK_z)
//             {
//                 m_audio_engine->decrease_octave();
//             }

//             if (m_current_event.key.keysym.sym == SDLK_x)
//             {
//                 m_audio_engine->increase_octave();
//             }

//             auto it = std::find(m_possible_keys.begin(), m_possible_keys.end(), m_current_event.key.keysym.sym);
//             if (it != m_possible_keys.end())
//             {
//                 Note note = key2note(m_current_event.key.keysym.sym);
//                 try_add_input_note(note);
//             }
//         }
//         if (m_current_event.type == SDL_KEYUP)
//         {
//             auto it = std::find(m_possible_keys.begin(), m_possible_keys.end(), m_current_event.key.keysym.sym);
//             if (it != m_possible_keys.end())
//             {
//                 Note note = key2note(m_current_event.key.keysym.sym);
//                 try_remove_input_note(note);
//             }
//         }   
//     }
// }

// bool App::try_add_input_note(Note note)
// {
//     auto it = std::find(m_pressed_notes.begin(), m_pressed_notes.end(), note);
//     if (it == m_pressed_notes.end())
//     {
//         m_pressed_notes.push_back(note);
//         m_audio_engine->update_input(m_pressed_notes);
//         return true;
//     }
//     return false;
// }

// bool App::try_remove_input_note(Note note)
// {
//     auto it = std::find(m_pressed_notes.begin(), m_pressed_notes.end(), note);
//     if (it != m_pressed_notes.end())
//     {
//         m_pressed_notes.erase(it);
//         m_audio_engine->update_input(m_pressed_notes);
//         return true;
//     }
//     return false;
// }