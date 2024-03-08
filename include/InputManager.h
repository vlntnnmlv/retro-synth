#pragma once

#include <map>
#include <SDL2/SDL.h>

class InputManager
{
    public:
        InputManager();
        ~InputManager();

        std::map<SDL_Keycode, bool> m_keysState;
        
        void processEvent(SDL_Event &i_event);
};