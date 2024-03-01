#include "RenderWindow.h"

RenderWindow::RenderWindow()
{}

RenderWindow::RenderWindow(const char* p_title, int p_w, int p_h)
{
    window = SDL_CreateWindow(p_title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, p_w, p_h, SDL_WINDOW_SHOWN);

    if (window == NULL) {
        std::cout << "Window initialization failed: " << SDL_GetError() << std::endl;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);

    if (renderer == NULL) {
        std::cout << "Renderer initialization failed: " << SDL_GetError() << std::endl;
    }
}

SDL_Texture* RenderWindow::loadTexture(const char* p_filePath)
{
    SDL_Texture *tex = IMG_LoadTexture(renderer, p_filePath);

    if (tex == NULL) {
        std::cout << "Load texture failed: " << SDL_GetError() << std::endl;
    }

    return tex;
}

void RenderWindow::cleanUp()
{
    SDL_DestroyWindow(window);
}

void RenderWindow::clear()
{
    SDL_RenderClear(renderer);
}

void RenderWindow::render(Entity& p_entity)
{
    SDL_Rect src;
    src.x = p_entity.getCurrentFrame().x;
    src.y = p_entity.getCurrentFrame().y;
    src.w = p_entity.getCurrentFrame().w;
    src.h = p_entity.getCurrentFrame().h;

    SDL_Rect dest;
    dest.x = p_entity.getPos().x;
    dest.y = p_entity.getPos().y;
    dest.w = src.w;
    dest.h = src.h;

    SDL_RenderCopy(renderer, p_entity.getTex(), &src, &dest);
}

void RenderWindow::render(int x, int y, SDL_Texture* p_tex)
{
    SDL_Rect src;
    src.x = 0;
    src.y = 0;

    SDL_QueryTexture(p_tex, NULL, NULL, &src.w, &src.h);

    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    dest.w = src.w;
    dest.h = src.h;

    SDL_RenderCopy(renderer, p_tex, &src, &dest);
}

void RenderWindow::render(float p_x, float p_y, const char* p_text, TTF_Font* p_font, SDL_Color p_textColor)
{
    SDL_Surface* surfaceMessage = TTF_RenderText_Blended(p_font, p_text, p_textColor);
    SDL_Texture* message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

    SDL_Rect src;
    src.x = 0;
    src.y = 0;
    src.w = surfaceMessage->w;
    src.h = surfaceMessage->h; 

    SDL_Rect dest;
    dest.x = p_x;
    dest.y = p_y;
    dest.w = src.w;
    dest.h = src.h;

    SDL_RenderCopy(renderer, message, &src, &dest);
    SDL_FreeSurface(surfaceMessage);
	SDL_DestroyTexture(message);
}

void RenderWindow::display()
{
    SDL_RenderPresent(renderer);
}