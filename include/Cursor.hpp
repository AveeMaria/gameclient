#pragma once

#include "Entity.hpp"

#include "SDL.h"
#include "SDL_image.h"

class Cursor : public Entity {
private:

public:
	Cursor();
	Cursor(const char* texturesheet);
	~Cursor();

	void Update();
	void Render();
};