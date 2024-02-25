// #include "SDLApp.h"

// SDLApp::SDLApp()
// {
//     init_window();
//     init_video();
// }

// SDLApp::~SDLApp()
// {
    
// }

// int SDLApp::init_window()
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

// int SDLApp::init_video()
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

//     glGenTextures(1, &m_gl_tex);
//     glBindTexture(GL_TEXTURE_2D, m_gl_tex);
//     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 512, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_BORDER);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_BORDER);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

//     return 0;
// }

// void SDLApp::start()
// {
//     m_running = true;

//     while (m_running)
//         loop();
// }

// void SDLApp::loop()
// {
//     Uint64 frame_start_time = SDL_GetPerformanceCounter();
    
//     poll_event();
//     render();

//     Uint64 frame_end_time = SDL_GetPerformanceCounter();
//     m_delta_time = (frame_end_time - frame_start_time) / (float)SDL_GetPerformanceFrequency();
//     m_time += m_delta_time;
// }

// void SDLApp::poll_event()
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

//                 // m_shader_unit->m_setter.set_2f("in_Resolution", m_window_width, m_window_height);
//             }
//         }

//         if (m_current_event.type == SDL_KEYDOWN)
//         {
//             if (m_current_event.key.keysym.sym == SDLK_ESCAPE)
//                 m_running = false;
//         }
//     }
// }

// void SDLApp::render()
// {
//     // Redraw everything
//     glClear(GL_COLOR_BUFFER_BIT);
//     glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
//     SDL_GL_SwapWindow(m_window);
// }