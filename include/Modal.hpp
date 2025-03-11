#pragma once

#include "Entity.hpp"

//modal za shop, upgrade in settingse
class Modal : public Entity {
private:
	short width = 0;
	short height = 0;

	std::string title = "";
	SDL_Rect titleRect = { 0, 0, 0, 0, };
	//dodej se exit gumber al key pa hide modal

public:
	//vsi modali bojo centered
	Modal();
	Modal(const char* title, short width, short height);
	Modal(std::string, short width, short height);

	~Modal();

	void setTitle(const char* t);
	void setTitle(std::string s);
	void centerModal();

	std::string getTitle() const;
	SDL_Rect getTitleRect();

	void Update() override;
	void Render() override;
};

