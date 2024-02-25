#include "GraphicCore.h"

#include <GL/glew.h>
#include <iostream>

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
}

GraphicCore::~GraphicCore()
{

}

void GraphicCore::start()
{
    m_running = true;
    
    m_mesh = new Mesh(1.0f);
    m_mesh2 = new Mesh(0.5f);
    m_shaderUnit = new ShaderUnit();

    while (m_running)
    {
        loop();
    }

    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

void GraphicCore::loop()
{
    while (SDL_PollEvent(&m_currentEvent) != 0)
    {
        if(m_currentEvent.type == SDL_QUIT)
            m_running = false;

        if (m_currentEvent.type == SDL_KEYDOWN)
        {
            if (m_currentEvent.key.keysym.sym == SDLK_ESCAPE)
                m_running = false;
        }
    }

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glUseProgram(m_shaderUnit->m_gl_program_id);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    m_mesh->draw();
    m_mesh2->draw();

    SDL_GL_SwapWindow(m_window);
}