#pragma once

#include <string>

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/glu.h>

#include "ShaderParameterSetter.h"

class ShaderUnit
{
    public:
        ShaderUnit();
        ~ShaderUnit();

        void   init_shaders();
        void   init_textures();

        ShaderParameterSetter m_setter;

        GLuint                   m_gl_program_id;

    private:
        std::string              *m_gl_vertex_shader_content;
        std::string              *m_gl_fragment_shader_content;

        GLuint                   m_gl_vertex_shader;
        GLuint                   m_gl_fragment_shader;

        GLint                    m_gl_vertex_pos_2d_location;
        GLuint                   m_gl_vao;
        GLuint                   m_gl_vbo;
        GLuint                   m_gl_ibo;
        GLuint                   m_gl_tex;
}; 