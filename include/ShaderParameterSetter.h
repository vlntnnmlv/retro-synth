#pragma once

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/glu.h>

class ShaderParameterSetter
{
    private:
        GLuint m_gl_program_id;

    public:
        ShaderParameterSetter(GLuint programm_id);
        ~ShaderParameterSetter();
  
        void set_1f(const char *parameter_name, GLfloat f1);
  
        void set_2f(const char *parameter_name, GLfloat f1, GLfloat f2);
  
        void set_3f(const char *parameter_name, GLfloat f1, GLfloat f2, GLfloat f3);
  
        void set_4f(const char *parameter_name, GLfloat f1, GLfloat f2, GLfloat f3, GLfloat f4);
  
        void set_1i(const char *parameter_name, GLint i1);
  
        void set_2i(const char *parameter_name, GLint i1, GLint i2);
  
        void set_3i(const char *parameter_name, GLint i1, GLint i2, GLint i3);
  
        void set_4i(const char *parameter_name, GLint i1, GLint i2, GLint i3, GLint i4);
  
        void set_1ui(const char *parameter_name, GLuint ui1);
  
        void set_2ui(const char *parameter_name, GLuint ui1, GLuint ui2);
  
        void set_3ui(const char *parameter_name, GLuint ui1, GLuint ui2, GLuint ui3);
  
        void set_4ui(const char *parameter_name, GLuint ui1, GLuint ui2, GLuint ui3, GLuint ui4);
  
        void set_1fv(const char *parameter_name, const GLfloat *fv1);
  
        void set_2fv(const char *parameter_name, const GLfloat *fv2);
  
        void set_3fv(const char *parameter_name, const GLfloat *fv3);
  
        void set_4fv(const char *parameter_name, const GLfloat *fv4);
  
        void set_1iv(const char *parameter_name, const GLint *iv1);
  
        void set_2iv(const char *parameter_name, const GLint *iv2);
  
        void set_3iv(const char *parameter_name, const GLint *iv3);
  
        void set_4iv(const char *parameter_name, const GLint *iv4);
  
        void set_1uiv(const char *parameter_name, const GLuint *uiv1);
  
        void set_2uiv(const char *parameter_name, const GLuint *uiv2);
  
        void set_3uiv(const char *parameter_name, const GLuint *uiv3);
  
        void set_4uiv(const char *parameter_name, const GLuint *uiv4);
};
