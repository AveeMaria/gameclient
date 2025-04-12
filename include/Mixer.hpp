#pragma once

#include <SDL_mixer.h>
#include <iostream>

class Mixer {
private:
	Mix_Music* music;
public:
	Mixer() {
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
			std::cerr << "SDL_mixer Error: " << Mix_GetError() << "\n";
		}

		music = Mix_LoadMUS("assets/sounds/Battle Preparations.mp3");
		if (!music) {
			std::cerr << "Failed to load music: " << Mix_GetError() << "\n";
		}
	}

	Mixer(const char* path) {
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
			std::cerr << "SDL_mixer Error: " << Mix_GetError() << "\n";
		}

		music = Mix_LoadMUS(path);
		if (!music) {
			std::cerr << "Failed to load music: " << Mix_GetError() << "\n";
		}
	}

	~Mixer() {
		Mix_FreeMusic(music);
	};

	void playMusic() {
		//Mix_Volume(-1, MIX_MAX_VOLUME / 2);
		Mix_PlayMusic(music, -1);
		Mix_VolumeMusic(MIX_MAX_VOLUME / 8);
	}
};