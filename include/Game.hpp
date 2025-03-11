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
#include "Modal.hpp"
#include "Cursor.hpp"
#include "Enemy.hpp"
#include "Tower.hpp"
#include "TextRenderer.hpp"
#include "Timer.hpp"
#include "Comms.hpp"

class Game
{
public:
	Game();
	~Game();

	void networking(Comms* comms);

	//void networking(Comms& comms);

	//void networking();

	void init(const char* title, int width, int height, bool fullscreen);

	bool isPaused() const { return paused; }

	void handleEvents();
	void update();
	void render();

	void clean();
	
	bool running() const { return isRunning; }

	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

private:
	//Comms comms;

	bool isRunning = false;
	bool paused = false;
	bool mouse_down = false;
	bool map_editor_mode = false;

	SDL_Event event = {};
	int mouseX = 0, mouseY = 0;

	SDL_Surface* icon = nullptr;

	const Uint32 debounceDelay = DEBOUNCE_DELAY;
	Uint32 lastMoveTime = 0;
	Uint32 lastclick = 0;

	uint32_t cnt = 0;
	//SDL_Window* window = nullptr;
};