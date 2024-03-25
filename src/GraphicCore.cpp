#include "GraphicCore.h"
#include "lodepng.h"

#include <GL/glew.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define CAMERA_MOVE_SPEED 0.3f;
#define FOV_CHANGE_RATE   0.1f;

GraphicCore::GraphicCore(int i_screenWidth, int i_screenHeight)
{
    m_screenWidth  = i_screenWidth;
    m_screenHeight = i_screenHeight;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL Failed Video Initialization: " << SDL_GetError() << "\n"; 
    }
    else
    {
        std::cout << "SDL Successfully initialized\n";
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    m_window = SDL_CreateWindow(
            "retro-synth",
            0,
            0,
            m_screenWidth,
            m_screenHeight,
            SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL
        );

    m_glContext = SDL_GL_CreateContext(m_window);

    if(SDL_GL_SetSwapInterval(1) < 0)
        std::cout << "Warning: Unable to set VSync! SDL Error: " << SDL_GetError() << "\n";

    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK)
        std::cout << "Error initializing GLEW!: " << glewGetErrorString(glewError) << "\n";

    // m_imageLoader = new ImageLoader(m_window);
}

GraphicCore::~GraphicCore()
{

}

void GraphicCore::start()
{
    m_running = true;

    m_cameraOffsetX = 0.0f;
    m_cameraOffsetY = 0.0f;
    m_cameraOffsetZ = 0.0f;

    m_FOV = 60.0f;

    // Create all objects    
    m_mesh = new Mesh(1.0f);
    m_mesh2 = new Mesh(1, 1, 10.0f);

    // Create all shaders
    m_shaderUnit = new ShaderUnit();

    // ----- TEXTURING -----
    GLint colAttrib = glGetAttribLocation(m_shaderUnit->m_gl_program_id, "color");
    glEnableVertexAttribArray(colAttrib);
    glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float),
            (void*) (2 * sizeof(GLfloat)));

    GLint texAttrib = glGetAttribLocation(m_shaderUnit->m_gl_program_id, "texcoord");
    glEnableVertexAttribArray(texAttrib);
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float),
            (void*) (5 * sizeof(GLfloat)));

    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);

    std::vector<unsigned char> image;
    unsigned width, height;
    unsigned error = lodepng::decode(image, width, height, "./textures/block.png");

    if (error != 0)
        std::cout << "error " << error << ": " << lodepng_error_text(error) << std::endl;

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, &image[0]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    m_textureUniformID = glGetUniformLocation(m_shaderUnit->m_gl_program_id, "in_texture");
    // ----- TEXTURING -----

    while (m_running)
    {
        loop();
    }

    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

void GraphicCore::loop()
{
    Uint64 frame_start_time = SDL_GetPerformanceCounter();

    while (SDL_PollEvent(&m_currentEvent) != 0)
    {
        if(m_currentEvent.type == SDL_QUIT)
            m_running = false;

        m_inputManager.processEvent(m_currentEvent);
    }

    if (m_inputManager.m_keysState[SDLK_ESCAPE])
        m_running = false;

    if (m_inputManager.m_keysState[SDLK_w])
        m_cameraOffsetZ += CAMERA_MOVE_SPEED;

    if (m_inputManager.m_keysState[SDLK_s])
        m_cameraOffsetZ -= CAMERA_MOVE_SPEED;

    if (m_inputManager.m_keysState[SDLK_a])
        m_cameraOffsetX += CAMERA_MOVE_SPEED;

    if (m_inputManager.m_keysState[SDLK_d])
        m_cameraOffsetX -= CAMERA_MOVE_SPEED;

    if (m_inputManager.m_keysState[SDLK_u])
        m_cameraOffsetY -= CAMERA_MOVE_SPEED;

    if (m_inputManager.m_keysState[SDLK_j])
        m_cameraOffsetY += CAMERA_MOVE_SPEED;

    if (m_inputManager.m_keysState[SDLK_i])
        m_FOV += FOV_CHANGE_RATE;

    if (m_inputManager.m_keysState[SDLK_o])
        m_FOV -= FOV_CHANGE_RATE;

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    // -- Camera settings --

    // model matrix
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(50.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    // view matrix
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(m_cameraOffsetX, m_cameraOffsetY, -15.0f + m_cameraOffsetZ));

    // projection matrix
    glm::mat4 projection = glm::perspective(glm::radians(m_FOV), float(m_screenWidth)/(float)m_screenHeight, 0.1f, 100.0f);

    int modelLoc = glGetUniformLocation(m_shaderUnit->m_gl_program_id, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); // TODO: move this setter to the generator

    int viewLoc = glGetUniformLocation(m_shaderUnit->m_gl_program_id, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    int projectionLoc = glGetUniformLocation(m_shaderUnit->m_gl_program_id, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // -- Camera settings --


    // --- USE TEXTURE
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    glUniform1i(m_textureUniformID, 0);
    // --- USE TEXTURE

    glUseProgram(m_shaderUnit->m_gl_program_id);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // m_shaderUnit->m_setter.set_3f("custom_color", 1.0f, 1.0f, 1.0f);

    // m_mesh->draw();

    m_shaderUnit->m_setter.set_3f("custom_color", 0.4f, 0.9f, 0.2f);

    m_mesh2->draw();

    SDL_GL_SwapWindow(m_window);

    Uint64 frame_end_time = SDL_GetPerformanceCounter();
    m_deltaTime = (frame_end_time - frame_start_time) / (float)SDL_GetPerformanceFrequency();
    m_time += m_deltaTime;
}