#include "../include/Projectile.hpp"

const char* typeTexture(const ProjType t) {
    switch (t) {
    case ProjType::ARROW:
        return "../../../assets/arrow.png";
    case ProjType::MAGEBALL:
        return "../../../assets/mageball.png";
    case ProjType::CANNONBALL:
        return "../../../assets/cannonball.png";
    default:
        return "../../../assets/projectile.png";
    }
}

Projectile::Projectile()
{
    type = ProjType::ARROW;
    std::cout << "WARNING: Nezeljen nacin konstukcije projektila.\n";
}

Projectile::Projectile(Coords start, ProjType p) {
    type = p;
    objTexture = std::make_unique<SDL_Texture*>(TextureManager::LoadTexture(typeTexture(type)));
    //objTexture = TextureManager::LoadTexture(typeTexture(type));
    Utils::convertCoords(xpos, ypos, start);
}

Projectile::Projectile(bool b) {
	type = ProjType::ARROW;
    objTexture = std::make_unique<SDL_Texture*>(TextureManager::LoadTexture(typeTexture(type)));
	//objTexture = TextureManager::LoadTexture(typeTexture(type));
}
Projectile::~Projectile()
{
    SDL_DestroyTexture(*objTexture);
}

bool Projectile::moveToTarget(const Coords& t) {
    
    //ce je zadel tarco da true da ga deleta iz vektorja
    if ((int)Utils::getDistance(Utils::rectMiddle(destRect), t) <= 16) {
        //std::cout << "projektil zadel tarco!\n";
        return true;
    }

    float dx = static_cast<float>(t.x - xpos);
    float dy = static_cast<float>(t.y - ypos);
    float angle = atan2(dy, dx);

    xpos += static_cast<short>(cos(angle) * speed);
    ypos += static_cast<short>(sin(angle) * speed);

    return false;
}

void Projectile::Update()
{
    srcRect.h = 32;
    srcRect.w = 32; 
    srcRect.x = 0;
    srcRect.y = 0;
    destRect.x = xpos;
    destRect.y = ypos;
    destRect.w = 16;
    destRect.h = 16;
}

void Projectile::Render()
{
	SDL_RenderCopy(Renderer::renderer, *objTexture, &srcRect, &destRect);
}
