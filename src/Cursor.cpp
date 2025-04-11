#include "../include/Cursor.hpp"

Cursor::Cursor()
{
	objTexture = TextureManager::LoadSharedTexture("../../../assets/cursor.png");

	srcRect = { 0, 0, 32, 32 };
	destRect = { 0, 0, 16, 16 };
}

Cursor::Cursor(const char* texturesheet)
{
	objTexture = TextureManager::LoadSharedTexture(texturesheet);
	
	srcRect = { 0, 0, 32, 32 };
	destRect = { 0, 0, 16, 16 };
}

Cursor::~Cursor()
{
}

void Cursor::Update()
{
	destRect.x = xpos;
	destRect.y = ypos;
}

void Cursor::Render()
{
	SDL_RenderCopy(Renderer::renderer, objTexture.get(), &srcRect, &destRect);
}
