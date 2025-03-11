#include "../include/TextRenderer.hpp"

TextRenderer::TextRenderer() : font(nullptr) {
    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf could not initialize! TTF_Error: " << TTF_GetError() << std::endl;
    }
}

TextRenderer::~TextRenderer() {
    if (font) {
        TTF_CloseFont(font);
    }
    TTF_Quit();
}

bool TextRenderer::loadFont(const std::string& fontPath, int fontSize) {
    font = TTF_OpenFont(fontPath.c_str(), fontSize);
    if (!font) {
        std::cerr << "ERROR: Failed to load font! TTF_Error: " << TTF_GetError() << std::endl;
        return false;
    }
    return true;
}

//text force formated for rektengel
bool TextRenderer::renderText(std::string text, SDL_Rect rect, Color c) {
    
    SDL_Color color = { c.red, c.green, c.blue, 255 };
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color);

    if (!textSurface) {
        std::cerr << "ERROR: Unable to render text surface! TTF_Error: " << TTF_GetError() << std::endl;
        return false;
    }

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(Renderer::renderer, textSurface);
    if (!textTexture) {
        std::cerr << "ERROR: Unable to create texture from surface! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(textSurface);
        return false;
    }

//to ne deletat, je ce hocs lep text sam ni contained na rect
    //rect.w = textSurface->w;
    //rect.h = textSurface->h;

    SDL_FreeSurface(textSurface);

    SDL_RenderCopy(Renderer::renderer, textTexture, NULL, &rect);
    SDL_DestroyTexture(textTexture);

    return true;
}

