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
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0) {
		std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
		return -1;
	}

	{
	int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG | IMG_INIT_TIF;
	if ((IMG_Init(imgFlags) & imgFlags) != imgFlags) {
		std::cerr << "IMG_Init failed: " << IMG_GetError() << std::endl;
		SDL_Quit();
		return -1;
	}
	}

	if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) == 0) {
		std::cerr << "Mix_Init failed: " << Mix_GetError() << std::endl;
		IMG_Quit();
		SDL_Quit();
		return -1;
	}

	if (TTF_Init() == -1) {
		std::cerr << "TTF_Init failed: " << TTF_GetError() << std::endl;
		Mix_Quit();
		IMG_Quit();
		SDL_Quit();
		return -1;
	}

	if (SDLNet_Init() != 0) {
		std::cerr << "SDLNet_Init failed: " << SDLNet_GetError() << std::endl;
		TTF_Quit();
		Mix_Quit();
		IMG_Quit();
		SDL_Quit();
		return -1;
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