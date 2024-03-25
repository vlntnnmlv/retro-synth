#pragma once

#include <SDL2/SDL.h>
#include <SDL_image.h>

class ImageLoader
{
    public:
        ImageLoader(SDL_Window *i_window)
        {
            m_renderer = SDL_CreateRenderer(i_window, -1, 0);
        }

        ~ImageLoader()
        {
        }

        SDL_Texture *load_texture(const char *i_file)
        {
            return IMG_LoadTexture(m_renderer, i_file);
        }

    private:    
        SDL_Renderer* m_renderer;
};