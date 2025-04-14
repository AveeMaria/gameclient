#pragma once

#include "renderer.hpp"

#include <iostream>
#include <time.h>
#include <vector>
#include <string>
#include <cstring>
#include <cmath>
#include <fstream>
#include <thread>
#include <queue>
#include <mutex>
#include <memory>
#include <chrono>
//#include <filesystem>

#include "TextureManager.hpp"
#include "Map.hpp"
#include "Utils.hpp"
#include "Entity.hpp"
#include "ShopModal.hpp"
#include "Cursor.hpp"
#include "Enemy.hpp"
#include "Tower.hpp"
#include "TextRenderer.hpp"
#include "Timer.hpp"
#include "Comms.hpp"
#include "EntityPlace.hpp"

class Game
{
public:
	Game();
	~Game();

	void networking(Comms* comms, UDPpacket* recvPacket);

	void init(const char* title, int width, int height, Uint8 gameID);

	bool isPaused() const { return paused; }

	void handleEvents();
	void update();
	void render();

	void clean();
	
	bool running() const { return isRunning; }

	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

	static uint8_t gameCnt;
	uint8_t getGameID() { return gameID; }
private:
	uint8_t gameID = 0;

	bool isRunning = false;
	bool paused = false;
	bool mouse_down = false;
	//bool map_editor_mode = false;

	bool defender = true;//true = defender, false = attacker

	SDL_Event event = {};
	int mouseX = 0, mouseY = 0;

	SDL_Surface* icon = nullptr;

	const Uint32 debounceDelay = DEBOUNCE_DELAY;
	Uint32 lastMoveTime = 0;
	Uint32 lastclick = 0;

	std::vector<int> deletedEntityIDs;

	std::vector<EnemyRequest> enemyRequests;
	std::vector<TowerRequest> towerRequests;

	uint32_t cnt = 0;

	std::unique_ptr<Timer> timer;

	std::unique_ptr<TextRenderer> textRenderer;

	std::unique_ptr<Map> map;

	Coords mouse_coords = { 0, 0 };

	std::vector<std::unique_ptr<Enemy>> enemies;

	std::vector<std::unique_ptr<Tower>> towers;

	std::unique_ptr<ShopModal> shop_modal;

	std::unique_ptr<Cursor> cursor;

	EntityPlace entity_place;
};