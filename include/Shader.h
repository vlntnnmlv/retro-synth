#pragma once

#include <string>

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/glu.h>

#include "Utility.h"
#include "AudioEngine.h"
#include <iostream>

class Shader
{
    public:
        Shader();
        Shader(AudioEngine *audio_engine, SDL_Window *window, float screen_width, float screen_height);
        ~Shader();

        void   render();
        GLuint get_program_id();

    private:
        AudioEngine              *m_audio_engine;
        SDL_Window               *m_window;

        std::string              *m_gl_vertex_shader_content;
        std::string              *m_gl_fragment_shader_content;

        GLuint                   m_gl_program_id;
        GLuint                   m_gl_vertex_shader;
        GLuint                   m_gl_fragment_shader;

        GLint                    m_gl_vertex_pos_2d_location;
        GLuint                   m_gl_vao;
        GLuint                   m_gl_vbo;
        GLuint                   m_gl_ibo;
        GLuint                   m_gl_tex;

}; 