#include "../include/Ally.hpp"

Ally::Ally() {
    std::cout << "ERROR: dont use default constructor\n";
    return;
}

Ally::Ally(const short x, const short y) {
    objTexture = TextureManager::LoadTexture("../../../assets/ally.png");

    srcRect = { 0, 0, TEXTURE_SIZE, TEXTURE_SIZE };

    this->xpos = x;
    this->ypos = y;
}

Ally::Ally(const Coords c) {
    objTexture = TextureManager::LoadTexture("../../../assets/ally.png");
    
    srcRect = { 0, 0, TEXTURE_SIZE, TEXTURE_SIZE };

    this->xpos = c.x;
    this->ypos = c.y;
}

Ally::~Ally() {

}

void Ally::Update()
{
    srcRect.h = TEXTURE_SIZE;
    srcRect.w = TEXTURE_SIZE;
    srcRect.x = 0;
    srcRect.y = 0;

    destRect.x = xpos;
    destRect.y = ypos;
    destRect.w = TILESIZE / 2;
    destRect.h = TILESIZE / 2;
}

void Ally::Render()
{
    //SDL_RenderDrawRect(Renderer::renderer, &destRect);
    SDL_RenderCopy(Renderer::renderer, objTexture, &srcRect, &destRect);
}