#include <iostream>

#include "ShaderUnit.h"
#include "Utility.h"
#include <SDL_image.h>


ShaderUnit::ShaderUnit()
{
    std::cout << "Creating shader unit...\n";

    m_gl_program_id = 0;
    m_gl_vertex_pos_2d_location = -1;
    m_gl_vao = 0;
    m_gl_vbo = 0;
    m_gl_ibo = 0;

    init_shaders();
    init_textures();

    m_setter = ShaderParameterSetter(m_gl_program_id);

    std::cout << "Shader unit created!\n";
}

ShaderUnit::~ShaderUnit()
{
    glUseProgram(0);
    glDisableVertexAttribArray(0);
    glDetachShader(m_gl_program_id, m_gl_vertex_shader);
    glDetachShader(m_gl_program_id, m_gl_fragment_shader);
    glDeleteProgram(m_gl_program_id);
    glDeleteShader(m_gl_vertex_shader);
    glDeleteShader(m_gl_fragment_shader);
    glDeleteTextures(1, &m_gl_tex);
    glDeleteBuffers(1, &m_gl_ibo);
    glDeleteBuffers(1, &m_gl_vbo);
    glDeleteVertexArrays(1, &m_gl_vao);
}

void ShaderUnit::init_shaders()
{
    GLint status;
    char err_buf[512];

    // Read vertex shader source
    m_gl_vertex_shader_content = read_file("shader/vertex.glsl");
    const char *vertex_shader_source = m_gl_vertex_shader_content->c_str();

    // Compile vertex shader
    m_gl_vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(m_gl_vertex_shader, 1, &vertex_shader_source, NULL);
    glCompileShader(m_gl_vertex_shader);

    // Check vertex shader
    glGetShaderiv(m_gl_vertex_shader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        glGetShaderInfoLog(m_gl_vertex_shader, sizeof(err_buf), NULL, err_buf);
        err_buf[sizeof(err_buf)-1] = '\0';
        fprintf(stderr, "Vertex shader compilation failed: %s\n", err_buf);
    }

    // Read fragment shader source
    m_gl_fragment_shader_content = read_file("shader/fragment.glsl");
    const char *fragment_shader_source = m_gl_fragment_shader_content->c_str();

    // Compile fragment shader
    m_gl_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(m_gl_fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(m_gl_fragment_shader);

    // Check fragment shader
    glGetShaderiv(m_gl_fragment_shader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        glGetShaderInfoLog(m_gl_fragment_shader, sizeof(err_buf), NULL, err_buf);
        err_buf[sizeof(err_buf)-1] = '\0';
        fprintf(stderr, "Fragment shader compilation failed: %s\n", err_buf);
    }

    // Link the program
    m_gl_program_id = glCreateProgram();
    glAttachShader(m_gl_program_id, m_gl_vertex_shader);
    glAttachShader(m_gl_program_id, m_gl_fragment_shader);
    glLinkProgram(m_gl_program_id);

    // Check the program
    glGetProgramiv(m_gl_program_id, GL_LINK_STATUS, &status);
    if (status != GL_TRUE)
    {
        glGetProgramInfoLog(m_gl_program_id, sizeof(err_buf), NULL, err_buf);
        err_buf[sizeof(err_buf)-1] = '\0';
        fprintf(stderr, "Program linkage failed: %s\n", err_buf);
    }

    // TODO: Research this logic
    glDetachShader(m_gl_program_id, m_gl_vertex_shader);
    glDetachShader(m_gl_program_id, m_gl_fragment_shader);

    glDeleteShader(m_gl_vertex_shader);
    glDeleteShader(m_gl_fragment_shader);
}

void ShaderUnit::init_textures()
{

}