#include "../include/Map.hpp"

/*
short lvl1[5][8] = {
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 }
};
*/

short lvl1[5][9] = {
    { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 1, 1, 1, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 1, 1, 1, 1, 1, 1, 1 },
    { 0, 0, 0, 0, 0, 0, 1, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 1, 0, 0 }
};

Map::Map() {
    water = TextureManager::LoadTexture("../../../assets/water.png");
    sand = TextureManager::LoadTexture("../../../assets/sand.png");
    grass = TextureManager::LoadTexture("../../../assets/grass.png");
    forest = TextureManager::LoadTexture("../../../assets/forest.png");
    corrupted = TextureManager::LoadTexture("../../../assets/corrupted.png");

    LoadMap(lvl1);
    
    //za drawmap da ve kam risat kvadratke
    src.x = src.y = 0;
    dest.x = dest.y = 0;
    src.w = 64; dest.w = TILESIZE;
    src.h = 64; dest.h = TILESIZE;
}

Map::~Map() {
    SDL_DestroyTexture(water);
    SDL_DestroyTexture(sand);
    SDL_DestroyTexture(grass);
    SDL_DestroyTexture(forest);
    SDL_DestroyTexture(corrupted);
}

short Map::getMapValue(short r, short c)
{
    return map[r][c];
    return 0;
}

void Map::LoadMap(short arr[MAP_ROWS][MAP_COLS]) {

    for (short row = 0; row < MAP_ROWS; row++) {
        for (short column = 0; column < MAP_COLS; column++) {
            map[row][column] = arr[row][column];
        }
    }
}

void Map::DrawMap() {
    short type = 0;

    for (short row = 0; row < MAP_ROWS; ++row) {
        for (short column = 0; column < MAP_COLS; ++column) {

            type = map[row][column];

            dest.x = column * TILESIZE;
            dest.y = row * TILESIZE;

            switch (type)
            {
            case 0:
                TextureManager::Draw(grass, src, dest);
                break;
            case 1:
                TextureManager::Draw(sand, src, dest);
                break;
            case 2:
                TextureManager::Draw(water, src, dest);
                break;
            case 3:
                TextureManager::Draw(forest, src, dest);
                break;
            default:
                TextureManager::Draw(corrupted, src, dest);
                break;
            }
        }
    }
}

void Map::paintValue(short x, short y)
{
    //std::cout << "x: " << x << " y: " << y << "\n";

    x /= TILESIZE;
    y /= TILESIZE;
    
    //std::cout << "col: " << x << " row: " << y << "\n";

    //da negre izven tabele
    if (x < 0 || y < 0) { return; }
    if (x > MAP_COLS - 1 || y > MAP_ROWS - 1) { return; }

    lvl1[y][x] = value;

    LoadMap(lvl1);
}

void Map::printMap()
{
    /*
    std::cout << "short lvl1[" << 10 << "][" << 15 << "] = {\n";

    for (short j = 0; j < 39; ++j) {
        std::cout << "\t{ ";

        for (short i = 0; i < 15; ++i) {
            std::cout << map[j][i] << ", ";
        }

        std::cout << "},\n";
    }

    std::cout << "\t{ ";
    for (short i = 0; i < 15; ++i) {
        std::cout << map[39][i] << ", ";
    }

    std::cout << "}";

    std::cout << "\n};\n";*/
}

short Map::getCol(short x)
{
	return x / TILESIZE;
}

short Map::getRow(short y)
{
	return y / TILESIZE;
}
