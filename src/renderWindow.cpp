#include "RenderWindow.h"
#include <vector>

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

void RenderWindow::render(Button& p_button)
{
    SDL_Rect src;
    src.x = p_button.getCurrentFrame().x;
    src.y = p_button.getCurrentFrame().y;
    src.w = p_button.getCurrentFrame().w;
    src.h = p_button.getCurrentFrame().h;
        
    SDL_Rect dest;
    dest.x = p_button.getPos().x;
    dest.y = p_button.getPos().y;
    dest.w = src.w;
    dest.h = src.h;

    SDL_RenderCopy(renderer, p_button.getTex(), &src, &dest);
}

void RenderWindow::render(Player& p_player, bool movingLeft)
{
    SDL_Rect src;
    src.x = p_player.getCurrentFrame().x;
    src.y = p_player.getCurrentFrame().y;
    src.w = p_player.getCurrentFrame().w;
    src.h = p_player.getCurrentFrame().h;

    SDL_Rect dest;
    dest.x = p_player.getPos().x;
    dest.y = p_player.getPos().y;
    dest.w = src.w;
    dest.h = src.h;

    SDL_RendererFlip flipType = SDL_FLIP_NONE;

    if (!movingLeft)
    {
        flipType = SDL_FLIP_HORIZONTAL;
    }

    SDL_RenderCopyEx(renderer, p_player.getTex(), &src, &dest, 0, NULL, flipType);
}

void RenderWindow::render(float p_x, float p_y, SDL_Texture* p_tex, bool center)
{
    SDL_Rect src;
    src.x = 0;
    src.y = 0;

    SDL_QueryTexture(p_tex, NULL, NULL, &src.w, &src.h);

    SDL_Rect dest;
    dest.w = src.w;
    dest.h = src.h;

    if (!center) {
        dest.x = p_x;
        dest.y = p_y;
    } else {
        dest.x = p_x - dest.w/2;
        dest.y = p_y - dest.h/2;
    }

    SDL_RenderCopy(renderer, p_tex, &src, &dest);
}

void RenderWindow::render(float p_x, float p_y, SDL_Texture* p_tex, double percent_w, bool center)
{
    SDL_Rect src;
    src.x = 0;
    src.y = 0;
    
    SDL_QueryTexture(p_tex, NULL, NULL, &src.w, &src.h);

    src.w = src.w * percent_w;

    SDL_Rect dest;
    dest.w = src.w;
    dest.h = src.h;

    if (!center) {
        dest.x = p_x;
        dest.y = p_y;
    } else {
        dest.x = p_x - dest.w/2;
        dest.y = p_y - dest.h/2;
    }

    SDL_RenderCopy(renderer, p_tex, &src, &dest);
}

void RenderWindow::render(float p_x, float p_y, const char* p_text, TTF_Font* p_font, SDL_Color p_textColor, bool center)
{
    SDL_Surface* surfaceMessage = TTF_RenderText_Blended(p_font, p_text, p_textColor);
    SDL_Texture* message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

    SDL_Rect src;
    src.x = 0;
    src.y = 0;
    src.w = surfaceMessage->w;
    src.h = surfaceMessage->h; 

    SDL_Rect dest;
    dest.w = src.w;
    dest.h = src.h;

    if (!center) {
        dest.x = p_x;
        dest.y = p_y;
    } else {
        dest.x = p_x - dest.w/2;
        dest.y = p_y - dest.h/2;
    }

    SDL_RenderCopy(renderer, message, &src, &dest);
    SDL_FreeSurface(surfaceMessage);
	SDL_DestroyTexture(message);
}

void RenderWindow::display()
{
    SDL_RenderPresent(renderer);
}