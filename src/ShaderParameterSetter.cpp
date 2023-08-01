#include "ShaderParameterSetter.h"

ShaderParameterSetter::ShaderParameterSetter(GLuint programm_id) : m_gl_program_id(programm_id)
{

}

ShaderParameterSetter::~ShaderParameterSetter()
{

}
  
void ShaderParameterSetter::set_1f(const char *parameter_name, GLfloat f1)
{
    glUniform1f(glGetUniformLocation(m_gl_program_id,parameter_name), f1);
}

  
void ShaderParameterSetter::set_2f(const char *parameter_name, GLfloat f1, GLfloat f2)
{
    glUniform2f(glGetUniformLocation(m_gl_program_id,parameter_name), f1, f2);
}

  
void ShaderParameterSetter::set_3f(const char *parameter_name, GLfloat f1, GLfloat f2, GLfloat f3)
{
    glUniform3f(glGetUniformLocation(m_gl_program_id,parameter_name), f1, f2, f3);
}

  
void ShaderParameterSetter::set_4f(const char *parameter_name, GLfloat f1, GLfloat f2, GLfloat f3, GLfloat f4)
{
    glUniform4f(glGetUniformLocation(m_gl_program_id,parameter_name), f1, f2, f3, f4);
}

  
void ShaderParameterSetter::set_1i(const char *parameter_name, GLint i1)
{
    glUniform1i(glGetUniformLocation(m_gl_program_id,parameter_name), i1);
}

  
void ShaderParameterSetter::set_2i(const char *parameter_name, GLint i1, GLint i2)
{
    glUniform2i(glGetUniformLocation(m_gl_program_id,parameter_name), i1, i2);
}

  
void ShaderParameterSetter::set_3i(const char *parameter_name, GLint i1, GLint i2, GLint i3)
{
    glUniform3i(glGetUniformLocation(m_gl_program_id,parameter_name), i1, i2, i3);
}

  
void ShaderParameterSetter::set_4i(const char *parameter_name, GLint i1, GLint i2, GLint i3, GLint i4)
{
    glUniform4i(glGetUniformLocation(m_gl_program_id,parameter_name), i1, i2, i3, i4);
}

  
void ShaderParameterSetter::set_1ui(const char *parameter_name, GLuint ui1)
{
    glUniform1ui(glGetUniformLocation(m_gl_program_id,parameter_name), ui1);
}

  
void ShaderParameterSetter::set_2ui(const char *parameter_name, GLuint ui1, GLuint ui2)
{
    glUniform2ui(glGetUniformLocation(m_gl_program_id,parameter_name), ui1, ui2);
}

  
void ShaderParameterSetter::set_3ui(const char *parameter_name, GLuint ui1, GLuint ui2, GLuint ui3)
{
    glUniform3ui(glGetUniformLocation(m_gl_program_id,parameter_name), ui1, ui2, ui3);
}

  
void ShaderParameterSetter::set_4ui(const char *parameter_name, GLuint ui1, GLuint ui2, GLuint ui3, GLuint ui4)
{
    glUniform4ui(glGetUniformLocation(m_gl_program_id,parameter_name), ui1, ui2, ui3, ui4);
}

  
void ShaderParameterSetter::set_1fv(const char *parameter_name, const GLfloat *fv1)
{
    glUniform1fv(glGetUniformLocation(m_gl_program_id,parameter_name), 1, fv1);
}

  
void ShaderParameterSetter::set_2fv(const char *parameter_name, const GLfloat *fv2)
{
    glUniform2fv(glGetUniformLocation(m_gl_program_id,parameter_name), 2, fv2);
}

  
void ShaderParameterSetter::set_3fv(const char *parameter_name, const GLfloat *fv3)
{
    glUniform3fv(glGetUniformLocation(m_gl_program_id,parameter_name), 3, fv3);
}

  
void ShaderParameterSetter::set_4fv(const char *parameter_name, const GLfloat *fv4)
{
    glUniform4fv(glGetUniformLocation(m_gl_program_id,parameter_name), 4, fv4);
}

  
void ShaderParameterSetter::set_1iv(const char *parameter_name, const GLint *iv1)
{
    glUniform1iv(glGetUniformLocation(m_gl_program_id,parameter_name), 1, iv1);
}

  
void ShaderParameterSetter::set_2iv(const char *parameter_name, const GLint *iv2)
{
    glUniform2iv(glGetUniformLocation(m_gl_program_id,parameter_name), 2, iv2);
}

  
void ShaderParameterSetter::set_3iv(const char *parameter_name, const GLint *iv3)
{
    glUniform3iv(glGetUniformLocation(m_gl_program_id,parameter_name), 3, iv3);
}

  
void ShaderParameterSetter::set_4iv(const char *parameter_name, const GLint *iv4)
{
    glUniform4iv(glGetUniformLocation(m_gl_program_id,parameter_name), 4, iv4);
}

  
void ShaderParameterSetter::set_1uiv(const char *parameter_name, const GLuint *uiv1)
{
    glUniform1uiv(glGetUniformLocation(m_gl_program_id,parameter_name), 1, uiv1);
}

  
void ShaderParameterSetter::set_2uiv(const char *parameter_name, const GLuint *uiv2)
{
    glUniform2uiv(glGetUniformLocation(m_gl_program_id,parameter_name), 2, uiv2);
}

  
void ShaderParameterSetter::set_3uiv(const char *parameter_name, const GLuint *uiv3)
{
    glUniform3uiv(glGetUniformLocation(m_gl_program_id,parameter_name), 3, uiv3);
}

  
void ShaderParameterSetter::set_4uiv(const char *parameter_name, const GLuint *uiv4)
{
    glUniform4uiv(glGetUniformLocation(m_gl_program_id,parameter_name), 4, uiv4);
}

