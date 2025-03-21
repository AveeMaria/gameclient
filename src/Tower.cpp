#include "../include/Tower.hpp"

uint32_t typeShootDelay(const TowerType &t) {
	switch (t) {
	case TowerType::ARCHER:
		return 250;
	case TowerType::MAGE:
		return 500;
	case TowerType::BARRACKS:
		return 750;//ta nima shootanja sploh
	case TowerType::MORTAR:
		return 1000;
	default:
		return 500;
	}
}

const char* typeTexture(const TowerType &t) {
	switch (t) {
		case TowerType::ARCHER:
			return "../../../assets/archer.png";
			break;
		case TowerType::MAGE:
			return "../../../assets/mage.png";
			break;
		case TowerType::BARRACKS:
			return "../../../assets/barracks.png";
			break;
		case TowerType::MORTAR:
			return "../../../assets/mortar.png";
			break;
		default:
			return "../../../assets/tower.png";
			break;
	}
}

//give korrekt projectile type based on tower type
ProjType projTypeFromTower(const TowerType &t) {
	switch (t) {
		case TowerType::ARCHER:
			return ProjType::ARROW;
			break;
		case TowerType::MAGE:
			return ProjType::MAGEBALL;
			break;
		case TowerType::BARRACKS:
			//ERROR barracks nc ne strelja!
			return ProjType::ARROW;
			break;
		case TowerType::MORTAR:
			return ProjType::CANNONBALL;
			break;
		default:
			return ProjType::ARROW;
			break;
	}
}

Tower::Tower()
{
	srcRect = { 0, 0, TEXTURE_SIZE, TEXTURE_SIZE };
	
	//random tip stolpa
    type = static_cast<TowerType>(rand() % 4);//jaful se casta int v int

	if (type == TowerType::BARRACKS) {
		allies.emplace_back(std::make_unique<Ally>( xpos, ypos + TILESIZE / 2 ));
	}

	//dobi praviln texture
	objTexture = std::make_unique<SDL_Texture*>(TextureManager::LoadTexture(typeTexture(type)));
	//objTexture = TextureManager::LoadTexture(typeTexture(type));
	
	Update();
}

Tower::Tower(short r, short c)
{
	srcRect = { 0, 0, TEXTURE_SIZE, TEXTURE_SIZE };
	
	xpos = r * TILESIZE;
	ypos = c * TILESIZE;

	type = static_cast<TowerType>(rand() % 4);//jaful se casta int v int

	objTexture = std::make_unique<SDL_Texture*>(TextureManager::LoadTexture(typeTexture(type)));
	//objTexture = TextureManager::LoadTexture(typeTexture(type));

	shootdelay = typeShootDelay(type);

	if (type == TowerType::BARRACKS) {
		allies.emplace_back(new Ally(100, 500));//TODO FIX KAJ SE KLE DOGAJA????????
	}
	
	Update();
}

Tower::Tower(TowerType t, short r, short c)
{
	srcRect = { 0, 0, TEXTURE_SIZE, TEXTURE_SIZE };

	xpos = r * TILESIZE;
	ypos = c * TILESIZE;
	
	type = t;
	
	objTexture = std::make_unique<SDL_Texture*>(TextureManager::LoadTexture(typeTexture(type)));
	//objTexture = TextureManager::LoadTexture(typeTexture(type));
	
	
	shootdelay = typeShootDelay(type);

	if (type == TowerType::BARRACKS) {
		allies.emplace_back(std::make_unique<Ally>(xpos + TILESIZE / 2, ypos + TILESIZE));
		allies.emplace_back(std::make_unique<Ally>(xpos + TILESIZE / 2, ypos + TILESIZE * 3 / 2));
		allies.emplace_back(std::make_unique<Ally>(xpos, ypos + TILESIZE));
		allies.emplace_back(std::make_unique<Ally>(xpos, ypos + TILESIZE * 3 / 2));
	}

	Update();
}

Tower::~Tower()
{
	SDL_DestroyTexture(*objTexture);
}

bool Tower::canShoot(uint32_t time)
{
	if (time - lastShootTime >= shootdelay) {
		//kr barracksi nimajo kej strelat
		if (type != TowerType::BARRACKS) {
			lastShootTime = time;
			return true;
		}
	}

	return false;
}

void Tower::makeProjecitle()
{
	projectiles.emplace_back(std::make_unique<Projectile>(Utils::rectMiddle(destRect), projTypeFromTower(type)));
	//projectiles.push_back(new Projectile(Coords{xpos + TILESIZE / 2, ypos + TILESIZE / 2 }));
}

void Tower::deleteProjectiles()
{
	projectiles.clear();
}

void Tower::setTargetCoords(Coords tc)
{
	target_coords = tc;
}

bool Tower::updateAllies() {
	if (type == TowerType::BARRACKS) {
		for (auto it = allies.begin(); it != allies.end(); ) {
			std::unique_ptr<Ally>& a = *it;
			a->Update();
			++it;
		}
	}
	return false;
}

// updata ALLY IN PROJEKTILE sam je DEPRECATED
bool Tower::moveProjectiles() {
	for (auto it = projectiles.begin(); it != projectiles.end(); ) {
		std::unique_ptr<Projectile>& p = *it;

		if (p->moveToTarget(target_coords)) {
			it = projectiles.erase(it);
			return true;//zadel prasca
		}
		else {
			++it;
		}
	}
	return false;
}

short Tower::calcDmg(ProjType t)
{
	switch (t) {
	case ProjType::ARROW:
		return 5;
		break;
	case ProjType::MAGEBALL:
		return 15;
		break;
	case ProjType::CANNONBALL:
		return 10;
		break;
	default:
		return 0;
		break;
	}

	return 0;
}

bool Tower::moveProjectiles(std::unique_ptr<Projectile>& proj) {
	for (auto it = projectiles.begin(); it != projectiles.end(); ) {
		std::unique_ptr<Projectile>& p = *it;

		if (p->moveToTarget(target_coords)) {
			proj = std::move(p);
			it = projectiles.erase(it);
			return true;
		}
		else {
			++it;
		}
	}
	return false;
}

void Tower::updateProjectiles() {
	for(auto& p : projectiles) {
		p->Update();
	}
}

void Tower::Update()
{
	destRect.x = xpos;
	destRect.y = ypos;
	destRect.w = TILESIZE;
	destRect.h = TILESIZE;
}

void Tower::Render()
{
	Utils::drawColoredCircle(xpos + TILESIZE / 2, ypos + TILESIZE / 2, range, { 0, 0, 0 });
	SDL_RenderCopy(Renderer::renderer, *objTexture, &srcRect, &destRect);
	
	for (auto& p : projectiles) {
		p->Render();
	}

	for (auto& a : allies) {
		a->Render();
	}
}