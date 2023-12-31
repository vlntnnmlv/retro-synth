#include "ShaderUnit.h"

ShaderUnit::ShaderUnit() {};

ShaderUnit::ShaderUnit(float window_width, float window_height)
{
    m_gl_program_id = 0;
    m_gl_vertex_pos_2d_location = -1;
    m_gl_vao = 0;
    m_gl_vbo = 0;
    m_gl_ibo = 0;

    init_shaders();
    init_geometry(window_width, window_height);
    init_textures();

    m_setter = ShaderParameterSetter(m_gl_program_id);
    m_setter.set_2f("in_Resolution", window_width, window_height);
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

    glGenVertexArrays(1, &m_gl_vao);
    glBindVertexArray(m_gl_vao);

    // Compile vertex shader
    m_gl_vertex_shader_content = read_file("shader/vertex.glsl");
    const char *vertex_shader_source = m_gl_vertex_shader_content->c_str();

    m_gl_vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(m_gl_vertex_shader, 1, &vertex_shader_source, NULL);
    glCompileShader(m_gl_vertex_shader);
    glGetShaderiv(m_gl_vertex_shader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        glGetShaderInfoLog(m_gl_vertex_shader, sizeof(err_buf), NULL, err_buf);
        err_buf[sizeof(err_buf)-1] = '\0';
        fprintf(stderr, "Vertex shader compilation failed: %s\n", err_buf);
    }

    // Compile fragment shader
    m_gl_fragment_shader_content = read_file("shader/fragment.glsl");
    const char *fragment_shader_source = m_gl_fragment_shader_content->c_str();

    m_gl_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(m_gl_fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(m_gl_fragment_shader);
    glGetShaderiv(m_gl_fragment_shader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        glGetShaderInfoLog(m_gl_fragment_shader, sizeof(err_buf), NULL, err_buf);
        err_buf[sizeof(err_buf)-1] = '\0';
        fprintf(stderr, "Fragment shader compilation failed: %s\n", err_buf);
    }

    // Link vertex and fragment shaders
    m_gl_program_id = glCreateProgram();
    glAttachShader(m_gl_program_id, m_gl_vertex_shader);
    glAttachShader(m_gl_program_id, m_gl_fragment_shader);

    glBindFragDataLocation(m_gl_program_id, 0, "out_Color");

    glLinkProgram(m_gl_program_id);
    glUseProgram(m_gl_program_id);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void ShaderUnit::init_geometry(float window_width, float window_height)
{
    // part of the screen to render!
    const GLfloat verts[4][4] = {
        //  x      y      s      t
        { -1.0f, -1.0f,  0.0f,  1.0f }, // BL
        { -1.0f,  1.0f,  0.0f,  0.0f }, // TL
        {  1.0f,  1.0f,  1.0f,  0.0f }, // TR
        {  1.0f, -1.0f,  1.0f,  1.0f }, // BR
    };

    // Populate vertex buffer
    glGenBuffers(1, &m_gl_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_gl_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    const GLint indicies[] = {
        0, 1, 2, 0, 2, 3
    };

    // Populate element buffer
    glGenBuffers(1, &m_gl_ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_gl_ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

    // Bind vertex position attribute
    GLint pos_attr_loc = glGetAttribLocation(m_gl_program_id, "in_Position");
    glVertexAttribPointer(pos_attr_loc, 2, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(pos_attr_loc);

    // Bind vertex texture coordinate attribute
    GLint tex_attr_loc = glGetAttribLocation(m_gl_program_id, "in_Texcoord");
    glVertexAttribPointer(tex_attr_loc, 2, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), (void*)(2*sizeof(GLfloat)));
    glEnableVertexAttribArray(tex_attr_loc);
}

void ShaderUnit::init_textures()
{

}