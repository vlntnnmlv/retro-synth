#include "Mesh.h"
#include <iostream>

Mesh::Mesh(float i_scale)
{
    std::cout << "Creating cube mesh...\n";
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

Mesh::Mesh(int numQuadsWidth, int numQuadsHeight, float quadSize)
{
    std::cout << "Creating multi-quad plane mesh...\n";
    glGenVertexArrays(1, &m_vertexArrayID);
    glBindVertexArray(m_vertexArrayID);

    // Total number of vertices across the width and height
    int numVerticesWidth = numQuadsWidth + 1;
    int numVerticesHeight = numQuadsHeight + 1;

    // Calculate the total number of vertices and indices
    int totalVertices = numVerticesWidth * numVerticesHeight;
    int totalQuads = numQuadsWidth * numQuadsHeight;
    int totalIndices = totalQuads * 6; // 6 indices per quad (2 triangles)

    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;

    // Generate vertices
    for (int y = 0; y < numVerticesHeight; ++y) {
        for (int x = 0; x < numVerticesWidth; ++x) {
            vertices.push_back(x * quadSize - (numQuadsWidth * quadSize / 2.0f)); // X
            vertices.push_back(0.0f);                                               // Y
            vertices.push_back(y * quadSize - (numQuadsHeight * quadSize / 2.0f)); // Z
        }
    }

    // Generate indices for each quad
    for (int y = 0; y < numQuadsHeight; ++y) {
        for (int x = 0; x < numQuadsWidth; ++x) {
            int topLeft = y * numVerticesWidth + x;
            int topRight = topLeft + 1;
            int bottomLeft = topLeft + numVerticesWidth;
            int bottomRight = bottomLeft + 1;

            // First triangle
            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);

            // Second triangle
            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }

    // Create buffers and upload the data
    glGenBuffers(1, &m_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &m_elementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

    m_vertexSize = indices.size();

    std::cout << "Multi-quad plane mesh created!\n";
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