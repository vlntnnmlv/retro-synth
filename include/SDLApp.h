// #pragma once

// #include <SDL2/SDL.h>
// #include <string>
// #include <vector>
// #include <cmath>
// #include <set>

// #include <GL/glew.h>
// #include <GL/glu.h>

// #include "AudioEngine.h"
// #include "ShaderUnit.h"

// class SDLApp
// {
//     public:
//         SDLApp();
//         ~SDLApp();

//         void start();


//     protected:
//         int init_window();
//         int init_video();

//         void loop();
//         void poll_event();
//         void render();

//         float         m_delta_time;
//         float         m_time;

//         SDL_GLContext m_gl_context;
//         ShaderUnit   *m_shader_unit;
//         GLuint        m_gl_tex;
//         SDL_Window*   m_window;
//         SDL_Event     m_current_event;

//         bool          m_running;
//         int           m_window_width;
//         int           m_window_height;
// };