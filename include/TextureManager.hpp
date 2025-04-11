#pragma once

#include "renderer.hpp"
#include <iostream>
#include <memory>
#include <map>

/*
class TextureManager {
private:
	//std::unique_ptr<SDL_Texture*> entity;
public:
	
	
	static SDL_Texture* LoadTexture(const char* filename);
	static void Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest);
	static SDL_Surface* LoadSurface(const char* filename);
};
*/

class TextureManager {
    static std::map<std::string, std::shared_ptr<SDL_Texture>> textures;
public:
    static std::shared_ptr<SDL_Texture> LoadSharedTexture(const std::string& filename) {
        if (textures.count(filename)) return textures[filename];

        SDL_Surface* tempSurface = IMG_Load(filename.c_str());
        if (!tempSurface) {
            std::cerr << "Failed to load surface: " << IMG_GetError() << "\n";
            return nullptr;
        }

        SDL_Texture* tex = SDL_CreateTextureFromSurface(Renderer::renderer, tempSurface);
        SDL_FreeSurface(tempSurface);

        if (!tex) {
            std::cerr << "Failed to create texture: " << SDL_GetError() << "\n";
            return nullptr;
        }

        auto sharedTex = std::shared_ptr<SDL_Texture>(tex, [](SDL_Texture* t) {
            SDL_DestroyTexture(t);
            });

        textures[filename] = sharedTex;
        return sharedTex;
    }

    static SDL_Texture* TextureManager::LoadTexture(const char* texture)
    {
        SDL_Surface* tempSurface = IMG_Load(texture);
        SDL_Texture* tex = SDL_CreateTextureFromSurface(Renderer::renderer, tempSurface);
        SDL_FreeSurface(tempSurface);

        if (tempSurface == nullptr) {
            std::cerr << "ERROR: Failed to load surface: " << texture << " SDL_image: " << IMG_GetError() << "\n";
        }

        if (tex == nullptr) {
            std::cerr << "ERROR: Failed to load texture: " << texture << " SDL_image: " << IMG_GetError() << "\n";
        }

        return tex;
    }

    static SDL_Surface* TextureManager::LoadSurface(const char* filename)
    {
        SDL_Surface* surface = IMG_Load(filename);
        if (surface != nullptr) {
            return surface;
        }

        std::cerr << "ERROR: Failed to load surface: " << filename << " SDL_image: " << IMG_GetError() << "\n";
        return nullptr;
    }
};