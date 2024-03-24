#pragma once

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/glu.h>

class Mesh
{
    public:
        Mesh(float i_scale); // identity cube
        Mesh(int i_width, int i_height, float i_scale); // plane
        ~Mesh();

        void draw();

        GLuint m_vertexArrayID;
        GLuint m_vertexBuffer;
        GLuint m_elementBuffer;
        GLuint m_vertexSize;
};