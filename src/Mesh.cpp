#include "Mesh.h"
#include <iostream>

Mesh::Mesh(float i_scale)
{
    std::cout << "Creating mesh...\n";
    glGenVertexArrays(1, &m_vertexArrayID);
    glBindVertexArray(m_vertexArrayID);

    GLfloat vertices[] = {
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
         0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f
    };

    GLint triangles[] = {
                          0, 2, 3, 0, 3, 1,
                          2, 6, 7, 2, 7, 3,
                          6, 4, 5, 6, 5, 7,
                          4, 0, 1, 4, 1, 5,
                          1, 3, 7, 1, 7, 5,
                          4, 6, 2, 4, 2, 0
                        };

    glGenBuffers(1, &m_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer); // setting this buffer active?
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &m_elementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangles), triangles, GL_STATIC_DRAW);

    m_vertexSize = sizeof(triangles) / sizeof(GLint);

    std::cout << "Mesh created!\n";
}

Mesh::~Mesh()
{

}

void Mesh::draw()
{
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer); // need to do this every time?
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementBuffer);
    glDrawElements(GL_TRIANGLES, m_vertexSize, GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(0);
}