#pragma once

#include "Entity.hpp"
#include <string>
#include <vector>

//modal za shop
class ShopModal : public Entity {
private:
	short width = 0;
	short height = 0;
	//textures
	std::unique_ptr<SDL_Texture*> tex1;
	std::unique_ptr<SDL_Texture*> tex2;
	std::unique_ptr<SDL_Texture*> tex3;
	std::unique_ptr<SDL_Texture*> tex4;
	//descriptions
	std::string title = "";
	std::string descText1a = "";
	std::string descText1b = "";
	std::string descText2a = "";
	std::string descText2b = "";
	std::string descText3a = "";
	std::string descText3b = "";
	std::string descText4a = "";
	std::string descText4b = "";

	SDL_Rect titleRect = { 0, 0, 0, 0, };
	SDL_Rect exitRect = { 0, 0, 0, 0 };
	//options
	SDL_Rect option1 = { 0, 0, 0, 0 };
	SDL_Rect option2 = { 0, 0, 0, 0 };
	SDL_Rect option3 = { 0, 0, 0, 0 };
	SDL_Rect option4 = { 0, 0, 0, 0 };
	//descriptions A
	SDL_Rect desc1a = { 0, 0, 0, 0 };
	SDL_Rect desc2a = { 0, 0, 0, 0 };
	SDL_Rect desc3a = { 0, 0, 0, 0 };
	SDL_Rect desc4a = { 0, 0, 0, 0 };
	//descriptions B
	SDL_Rect desc1b = { 0, 0, 0, 0 };
	SDL_Rect desc2b = { 0, 0, 0, 0 };
	SDL_Rect desc3b = { 0, 0, 0, 0 };
	SDL_Rect desc4b = { 0, 0, 0, 0 };
	//texture size
	SDL_Rect texutreSrcRect = { 0,0,128,128 };
public:
	ShopModal();
	ShopModal(const char* title, bool r, short Tile_width, short Tile_height);
	ShopModal(std::string title, bool r, short Tile_width, short Tile_height);
	~ShopModal();

	SDL_Rect getOptionRect(int r) const;
	SDL_Rect getDescRectA(int r) const;
	SDL_Rect getDescRectB(int r) const;

	void setTitle(const char* t);
	void setTitle(std::string s);
	void centerModal();

	std::vector<std::string> getDescriptionsA() const;
	std::vector<std::string> getDescriptionsB() const;

	std::string getTitle() const;

	SDL_Rect getTitleRect();
	SDL_Rect getExitRect() const { return exitRect; }

	SDL_Rect getOption1() const { return option1; }
	SDL_Rect getOption2() const { return option2; }
	SDL_Rect getOption3() const { return option3; }
	SDL_Rect getOption4() const { return option4; }

	int getSelectedOption(Coords c);

	void Update() override;
	void Render() override;
};

