#pragma once

#include "renderer.hpp"
#include "Utils.hpp"

class TextRenderer {
public:
    TextRenderer();
    ~TextRenderer();

    bool loadFont(const std::string& fontPath, int fontSize);

    bool renderText(std::string text, SDL_Rect rect, Color c = { 255, 80, 80 });
    //bool renderText(std::string text, SDL_Rect rect, Color c);
    //bool renderForcedText(std::string text, SDL_Rect rect, Color c);
private:
    TTF_Font* font;
};