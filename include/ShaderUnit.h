#pragma once

#include <string>

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/glu.h>

#include "Utility.h"
#include "AudioEngine.h"
#include <iostream>

class ShaderUnit
{
    public:
        ShaderUnit();
        ShaderUnit(AudioEngine *audio_engine, SDL_Window *window, float screen_width, float screen_height);
        ~ShaderUnit();

        void   init_shaders();
        void   init_geometry(float window_width, float window_height);
        void   init_textures();
        void   render();

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