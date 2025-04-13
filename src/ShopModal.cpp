#include "../include/ShopModal.hpp"

ShopModal::ShopModal()
{
	srcRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	destRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	
	objTexture = TextureManager::LoadSharedTexture("../../../assets/modal.png");

	std::cout << "WARNING: nezeljen modal konstruktor\n";
}

ShopModal::ShopModal(const char* title, bool r, short Tile_width, short Tile_height)
{
	this->title = title;
	this->width = Tile_width * TILESIZE;
	this->height = Tile_height * TILESIZE;

	if (width >= SCREEN_WIDTH || width <= 0) {
		std::cerr << "ERROR: Invalid modal width!!\n";
		return;
	}

	if (height >= SCREEN_HEIGHT || height <= 0) {
		std::cerr << "ERROR: Invalid modal height!!\n";
		return;
	}
	objTexture = TextureManager::LoadSharedTexture("../../../assets/modal.png");

	if (r) {
		std::cout << "OK: defender modal\n";

		tex1 = std::make_unique<SDL_Texture*>(TextureManager::LoadTexture("../../../assets/archer.png"));
		tex2 = std::make_unique<SDL_Texture*>(TextureManager::LoadTexture("../../../assets/mage.png"));
		tex3 = std::make_unique<SDL_Texture*>(TextureManager::LoadTexture("../../../assets/barracks.png"));
		tex4 = std::make_unique<SDL_Texture*>(TextureManager::LoadTexture("../../../assets/mortar.png"));
		
		descText1a = "Archer";
		descText1b = "Fast 70C";

		descText2a = "Mage";
		descText2b = "Damage 90C";

		descText3a = "Barraks";
		descText3b = "Block 70 C";

		descText4a = "Mortar";
		descText4b = "Area 100C";
	}
	else {
		std::cout << "OK: attacker modal\n";

		tex1 = std::make_unique<SDL_Texture*>(TextureManager::LoadTexture("../../../assets/goblin.png"));
		tex2 = std::make_unique<SDL_Texture*>(TextureManager::LoadTexture("../../../assets/thief.png"));
		tex3 = std::make_unique<SDL_Texture*>(TextureManager::LoadTexture("../../../assets/bandit.png"));
		tex4 = std::make_unique<SDL_Texture*>(TextureManager::LoadTexture("../../../assets/knight.png"));
	
		descText1a = "Goblin";
		descText1b = "Swift 10C";
		
		descText2a = "Thief";
		descText2b = "Sneaky 20C";

		descText3a = "Bandit";
		descText3b = "Armed 30C";


		descText4a = "Knight";
		descText4b = "Armored 50C";
	}

	Update();//modal se rab sam enkat updatat?
}

ShopModal::ShopModal(std::string title, bool r, short Tile_width, short Tile_height)
{
	this->title = title;
	this->width = Tile_width * TILESIZE;
	this->height = Tile_height * TILESIZE;

	if (width >= SCREEN_WIDTH || width <= 0) {
		std::cerr << "ERROR: Invalid modal width!!\n";
		return;
	}

	if (height >= SCREEN_HEIGHT || height <= 0) {
		std::cerr << "ERROR: Invalid modal height!!\n";
		return;
	}

	objTexture = TextureManager::LoadSharedTexture("../../../assets/modal.png");

	Update();//modal se rab sam enkat updatat?
}

ShopModal::~ShopModal()
{
}

void ShopModal::setTitle(const char* t)
{
	title = t;
}

std::string ShopModal::getTitle() const { return title; }

SDL_Rect ShopModal::getTitleRect() {
	if (this == nullptr) {
		std::cout << "ERROR: ShopModal is null pointer!";
		return SDL_Rect{ 0,0,0,0 };
	}
	return titleRect;
}

void ShopModal::setTitle(std::string t)
{
	title = t;
}


int ShopModal::getSelectedOption(Coords c)
{
	if (Utils::coordInRect(c, exitRect)) {
		return -1;
	}
	else if (Utils::coordInRect(c, option1)) {
		return 1;
	}
	else if (Utils::coordInRect(c, option2)) {
		return 2;
	}
	else if (Utils::coordInRect(c, option3)) {
		return 3;
	}
	else if (Utils::coordInRect(c, option4)) {
		return 4;
	}

	return 0;
}

void ShopModal::Update()
{
	centerModal();//centrira se

	titleRect = { xpos,ypos, 9 * width / 10 , height / 6 };
	exitRect = { xpos + 9 * width / 10,ypos, width / 10 , height / 6 };

	//Options//
	// o1 o2 //
	// o3 o4 //
	///////////

	option1 = { xpos + width / 10, ypos + TILESIZE, TILESIZE , TILESIZE };
	//desc1 = { xpos + 3 * width / 10, ypos + TILESIZE, TILESIZE , TILESIZE };
	desc1a = { xpos + 3 * width / 10, ypos + TILESIZE, TILESIZE , TILESIZE / 2 };
	desc1b = { xpos + 3 * width / 10, ypos + 3 * TILESIZE / 2 , TILESIZE , TILESIZE / 2 };
	
	option3 = { xpos + 1 * width / 10, ypos + 2 * TILESIZE, TILESIZE , TILESIZE };
	//desc3 = { xpos + 3 * width / 10, ypos + 2 * TILESIZE, TILESIZE , TILESIZE };
	desc3a = { xpos + 3 * width / 10, ypos + 2 * TILESIZE, TILESIZE , TILESIZE / 2 };
	desc3b = { xpos + 3 * width / 10, ypos + 5 * TILESIZE / 2, TILESIZE , TILESIZE / 2 };

	option2 = { xpos + 1 * width / 2, ypos + TILESIZE, TILESIZE , TILESIZE };
	//desc2 = { xpos + 7 * width / 10, ypos + TILESIZE, TILESIZE , TILESIZE };
	desc2a = { xpos + 7 * width / 10, ypos + TILESIZE, TILESIZE , TILESIZE / 2};
	desc2b = { xpos + 7 * width / 10, ypos + 3 * TILESIZE / 2, TILESIZE , TILESIZE / 2};

	option4 = { xpos + 1 * width / 2, ypos + 2 * TILESIZE, TILESIZE , TILESIZE };
	//desc4 = { xpos + 7 * width / 10, ypos + 2 * TILESIZE, TILESIZE , TILESIZE };
	desc4a = { xpos + 7 * width / 10, ypos + 2 * TILESIZE, TILESIZE , TILESIZE / 2 };
	desc4b = { xpos + 7 * width / 10, ypos + 5 * TILESIZE / 2, TILESIZE , TILESIZE / 2 };

    srcRect.h = 576;//TODO FIX
    srcRect.w = 360;
    srcRect.x = 0;
    srcRect.y = 0;

    destRect.x = xpos;
    destRect.y = ypos;
    destRect.w = width;
    destRect.h = height;
}

void ShopModal::centerModal() {
	xpos = (SCREEN_WIDTH - width) * 0.5;
	ypos = (SCREEN_HEIGHT - height) * 0.5;
}

SDL_Rect ShopModal::getOptionRect(int r) const {
	switch (r) {
	case 1:
		return option1;
	case 2:
		return option2;
	case 3:
		return option3;
	case 4:
		return option4;
	default:
		return SDL_Rect{ 0, 0, 0, 0 };
	}
}

SDL_Rect ShopModal::getDescRectA(int r) const
{
	switch (r) {
	case 1:
		return desc1a;
	case 2:
		return desc2a;
	case 3:
		return desc3a;
	case 4:
		return desc4a;
	default:
		return SDL_Rect{ 0, 0, 0, 0 };
	}
}

SDL_Rect ShopModal::getDescRectB(int r) const {
	switch (r) {
	case 1:
		return desc1b;
	case 2:
		return desc2b;
	case 3:
		return desc3b;
	case 4:
		return desc4b;
	default:
		return SDL_Rect{ 0, 0, 0, 0 };
	}
}

std::vector<std::string> ShopModal::getDescriptionsA() const {
	return { descText1a, descText2a, descText3a, descText4a };
}

std::vector<std::string> ShopModal::getDescriptionsB() const {
	return { descText1b, descText2b, descText3b, descText4b };
}

void ShopModal::Render()
{
	/*
    SDL_Rect rect = { (int)xpos, (int)ypos, (int)width, (int)height };  
    SDL_RenderDrawRect(Renderer::renderer, &rect);
	*/

	SDL_RenderCopy(Renderer::renderer, objTexture.get(), &srcRect, &destRect);
	
	SDL_RenderDrawLine(Renderer::renderer, xpos, ypos + TILESIZE * 0.5, xpos + width, ypos + TILESIZE * 0.5);

	SDL_RenderDrawRect(Renderer::renderer, &exitRect);
	
    SDL_RenderCopy(Renderer::renderer, *tex1, &texutreSrcRect, &option1);
	SDL_RenderCopy(Renderer::renderer, *tex2, &texutreSrcRect, &option2);
	SDL_RenderCopy(Renderer::renderer, *tex3, &texutreSrcRect, &option3);
	SDL_RenderCopy(Renderer::renderer, *tex4, &texutreSrcRect, &option4);
	
}
