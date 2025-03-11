#include "../include/Entity.hpp"

Entity::Entity(const char* texturesheet, short x, short y)
{
    objTexture = TextureManager::LoadTexture(texturesheet);
    xpos = x;
    ypos = y;
    Update();
}

void Entity::Update()
{
    srcRect.h = TILESIZE;
    srcRect.w = TILESIZE;
    srcRect.x = 0;
    srcRect.y = 0;

    destRect.x = xpos;
    destRect.y = ypos;
    destRect.w = srcRect.w;
    destRect.h = srcRect.h;
}

void Entity::Render()
{
    SDL_RenderCopy(Renderer::renderer, objTexture, &srcRect, &destRect);
}

Entity::Entity(Tile t) : xpos(t.col* TILESIZE), ypos(t.row* TILESIZE), objTexture(nullptr), srcRect{ 0, 0, TILESIZE, TILESIZE }, destRect{ 0, 0, TILESIZE, TILESIZE } {
    ++ent_cnt;
    //Update();
}

Entity::Entity(Coords c)  : xpos(c.x), ypos(c.y), objTexture(nullptr), srcRect{ 0, 0, TILESIZE, TILESIZE }, destRect{ 0, 0, TILESIZE, TILESIZE } {
    ++ent_cnt;
    //Update();
}

Coords Entity::getCoords() const {
	return Coords{ xpos, ypos };
}

SDL_Rect Entity::getRect() const {
	return destRect;
}