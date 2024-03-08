#include "InputManager.h"

InputManager::InputManager()
{

}

InputManager::~InputManager()
{
    
}

void InputManager::processEvent(SDL_Event &i_event)
{
    if (i_event.type == SDL_KEYDOWN)
        m_keysState[i_event.key.keysym.sym] = true;

    if (i_event.type == SDL_KEYUP)
            m_keysState[i_event.key.keysym.sym] = false;
}