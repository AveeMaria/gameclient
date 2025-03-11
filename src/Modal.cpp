#include "../include/Modal.hpp"

Modal::Modal()
{
	srcRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	destRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	objTexture = TextureManager::LoadTexture("../../../assets/modal.png");
	std::cout << "WARNING: nezeljen modal konstruktor\n";
}

Modal::Modal(const char* title, short width, short height)
{
	objTexture = TextureManager::LoadTexture("../../../assets/modal.png");

	if (width >= SCREEN_WIDTH || width <= 0) {
		std::cerr << "ERROR: Invalid modal width!!\n";
		return;
	}

	if (height >= SCREEN_HEIGHT || height <= 0) {
		std::cerr << "ERROR: Invalid modal height!!\n";
		return;
	}

	this->title = title;
	this->width = width;
	this->height = height;


	Update();//modal se rab sam enkat updatat?
}

Modal::Modal(std::string title, short width, short height)
{
	objTexture = TextureManager::LoadTexture("../../../assets/modal.png");

	if (width >= SCREEN_WIDTH || width <= 0) {
		std::cerr << "ERROR: Invalid modal width!!\n";
		return;
	}

	if (height >= SCREEN_HEIGHT || height <= 0) {
		std::cerr << "ERROR: Invalid modal height!!\n";
		return;
	}

	this->title = title;
	this->width = width;
	this->height = height;

	Update();//modal se rab sam enkat updatat?
}

Modal::~Modal()
{
	SDL_DestroyTexture(objTexture);
}

void Modal::setTitle(const char* t)
{
	title = t;
}

std::string Modal::getTitle() const { return title; }


SDL_Rect Modal::getTitleRect() {
	if (this == nullptr) {
		std::cout << "ERROR: Modal is null pointer!";
		return SDL_Rect{ 0,0,0,0 };
	}
	return titleRect;
}


void Modal::setTitle(std::string t)
{
	title = t;
}


void Modal::Update()
{
	centerModal();

	titleRect = { xpos,ypos, width, height / 5 };

    srcRect.h = 576;//TODO FIX
    srcRect.w = 360;
    srcRect.x = 0;
    srcRect.y = 0;

    destRect.x = xpos;
    destRect.y = ypos;
    destRect.w = width;
    destRect.h = height;
}

void Modal::centerModal() {
	xpos = (SCREEN_WIDTH - width) / 2;
	ypos = (SCREEN_HEIGHT - height) / 2;
}

void Modal::Render()
{
	SDL_RenderCopy(Renderer::renderer, objTexture, &srcRect, &destRect);
}
