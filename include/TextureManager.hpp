#pragma once

#include "renderer.hpp"
#include <iostream>
#include <memory>

class TextureManager {
private:
	//std::unique_ptr<SDL_Texture*> entity;
public:
	
	
	static SDL_Texture* LoadTexture(const char* filename);
	static void Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest);
	static SDL_Surface* LoadSurface(const char* filename);
};