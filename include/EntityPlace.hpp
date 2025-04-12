#pragma once

#include "TextureManager.hpp">"
#include <memory>

struct EntityPlace {
private:
    std::unique_ptr<SDL_Texture*> texture;//to narise entity pod cursorjem
    int type;

	SDL_Rect destRect = { 0, 0, TILESIZE, TILESIZE };
	SDL_Rect srcRect = { 0, 0, 128, 128 };
public:
	void setEntity(std::unique_ptr<SDL_Texture*> tex, int type) {
		this->texture = std::move(tex);
		this->type = type;
	}

	bool isSet() const {
		return texture != nullptr;
	}

	void deleteTex() {
		texture.reset();
	}

	void setDestRect(int x, int y, int w, int h) {
		destRect.x = x;
		destRect.y = y;
		destRect.w = w;
		destRect.h = h;
	}

	int getType() const { return type; }

	void Render() {
		SDL_RenderCopy(Renderer::renderer, *texture, &srcRect, &destRect);
	}
};