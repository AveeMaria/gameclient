#include "./include/Network.hpp"
#include "./include/PacketType.hpp"

#include "./include/Comms.hpp"

#include "include/Utils.hpp"

#include "./include/Game.hpp"
#include "./include/config.hpp"
#include "./include/renderer.hpp"


//SDL_Renderer* Renderer::renderer = nullptr;
//SDL_Window* Renderer::window = nullptr;

Game game;

int main(int argc, char* argv[])
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) < 0)
	{
		std::cerr << "ERROR: SDL could not initialize! SDL_Error: " << SDL_GetError() << "\n";
	}
	if (TTF_Init() == -1) {
		std::cout << "ERROR: SDL_ttf could not initialize! TTF_Error: " << TTF_GetError() << "\n";
	}

	if (SDLNet_Init() < 0) {
		std::cerr << "ERROR: SDLNet_Init failed: " << SDLNet_GetError() << "\n";
		SDLNet_Quit();
	}


	Renderer::window = SDL_CreateWindow("Vojna kraljestev", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	if (Renderer::window == nullptr) {
		std::cerr << "Error creating window: " << SDL_GetError() << std::endl;
		return -1;
	}

	Renderer::renderer = SDL_CreateRenderer(Renderer::window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (Renderer::renderer == nullptr) {
		std::cerr << "Error creating renderer: " << SDL_GetError() << std::endl;
		return -1;
	}

	if (Renderer::renderer) {
		SDL_SetRenderDrawColor(Renderer::renderer, 255, 255, 255, 255);
		//Utils::drawLoadingScreen();

		//simuliramo slow load time
		//std::this_thread::sleep_for(std::chrono::milliseconds(300));
	}




	Comms comms("127.0.0.1", (Uint16)12346);

	srand(static_cast<unsigned int>(time(nullptr)));

	const int frameDelay = 1000 / FPS;

	Uint32 frameStart;
	int frameTime;

	//za racunat FPS
	int frameCount = 0;
	Uint32 fpsTimer = SDL_GetTicks();
	
	/*
	if (comms.stack_send(SYN{ 0 })) {
		std::cout << "SYN SENT\n";
	}*/

	game.init("Vojna kraljestev", SCREEN_WIDTH, SCREEN_HEIGHT, false);

	std::this_thread::sleep_for(std::chrono::milliseconds(5));
	
	if (comms.stack_send(SYN{ 0 })) {
		std::cout << "SYN SENT\n";
	}
	
	while (game.running())
	{
		frameStart = SDL_GetTicks();

		game.handleEvents();
		//game.networking(&comms);
		game.update();
		game.render();

		frameTime = SDL_GetTicks() - frameStart;

		//Racuna FPS
		frameCount++;
		if (SDL_GetTicks() - fpsTimer >= 1000) {
			//std::cout << "FPS: " << frameCount << "\n";
			frameCount = 0;//to je fps
			fpsTimer = SDL_GetTicks();
		}

		if (frameDelay > frameTime) {
			SDL_Delay(frameDelay - frameTime);
		}
	}

	game.clean();
	
	if (Renderer::window) {
		SDL_DestroyWindow(Renderer::window);
		Renderer::window = nullptr;
	}

	if (Renderer::renderer) {
		SDL_DestroyRenderer(Renderer::renderer);
		Renderer::renderer = nullptr;
	}


	TTF_Quit();
	SDL_Quit();


	std::this_thread::sleep_for(std::chrono::milliseconds(200000));

	return 0;
}