#pragma once

#include <iostream>

#include "renderer.hpp"

#include "config.hpp"
#include "TextureManager.hpp"

class Map {
public:
    Map();
    ~Map();

    short getMapValue(short r, short c);

    void LoadMap(short arr[MAP_ROWS][MAP_COLS]);
    void DrawMap();

    void paintValue(short x, short y);
    void printMap();

	//TODO KORDS TO ROWS AND COLS
    
    short getCol(short x);
	short getRow(short y);

    short value = 0;
private:
    SDL_Rect src, dest;

    std::unique_ptr<SDL_Texture*> water;
    std::unique_ptr<SDL_Texture*> sand;
    std::unique_ptr<SDL_Texture*> grass;
    std::unique_ptr<SDL_Texture*> forest;
    std::unique_ptr<SDL_Texture*> corrupted;


    /*
    SDL_Texture* water;
    SDL_Texture* sand;
    SDL_Texture* grass;
    SDL_Texture* forest;
    SDL_Texture* corrupted;
*/
    short map[MAP_ROWS][MAP_COLS];
};