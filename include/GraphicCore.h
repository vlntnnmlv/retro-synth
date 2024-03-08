#pragma once

#include <SDL2/SDL.h>

#include "InputManager.h"

#include "Mesh.h"
#include "ShaderUnit.h"

class GraphicCore
{
    public:
        GraphicCore(int i_screenWidth, int i_screenHeight);
        ~GraphicCore();

        Mesh       *m_mesh;
        Mesh       *m_mesh2;
        ShaderUnit *m_shaderUnit;

        float m_deltaTime;
        float m_time;

        void start();
        void loop();
        bool m_running;
        SDL_Event m_currentEvent;

        SDL_Window *m_window = nullptr;
        SDL_GLContext m_glContext = nullptr;

        int m_screenWidth;
        int m_screenHeight;

        float m_cameraOffsetX;
        float m_cameraOffsetY;
        float m_cameraOffsetZ;
        float m_FOV;

        InputManager m_inputManager;
};