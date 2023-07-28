#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include <cmath>
#include <set>

#include <GL/glew.h>
#include <GL/glu.h>

#include "AudioEngine.h"

class App
{
    public:
        App();
        void start();

    private:
        ~App();

        int init();
        int init_audio();
        int init_window();
        int init_video();
        int init_shaders();
        int init_geometry();
        int init_textures();

        void loop();
        void poll_event();
        void render();

        SDL_AudioDeviceID        m_audio_device_id;
        SDL_Window*              m_window;
        SDL_Event                m_current_event;
        SDL_Keycode              m_current_key;
        int                      m_window_width;
        int                      m_window_height;
        bool                     m_running;
        AudioEngine              m_audio_engine;
        std::set<SDL_Keycode>    m_pressed_keys;

        std::vector<SDL_Keycode> m_possible_keys;

        SDL_GLContext            m_gl_context;
        GLuint                   m_gl_program_id;
        GLuint                   m_gl_vertex_shader;
        GLuint                   m_gl_fragment_shader;
        std::string              *m_gl_vertex_shader_content;
        std::string              *m_gl_fragment_shader_content;

        GLint                    m_gl_vertex_pos_2d_location;
        GLuint                   m_gl_vao;
        GLuint                   m_gl_vbo;
        GLuint                   m_gl_ibo;
        GLuint                   m_gl_tex;

        float                    m_time;
        float                    m_delta_time;
};