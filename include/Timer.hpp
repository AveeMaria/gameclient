#pragma once

#include "Entity.hpp"
#include "TextRenderer.hpp"
#include <sstream>

class Timer {
private:
	uint32_t start_time;
	uint32_t curr_time;

	SDL_Rect rect = { 512, 0, 128, 64 };

	uint32_t seconds;//kok sekund (auto mnozi z 1000) 
	bool finished;
public:
	Timer();
	Timer(uint32_t sec);
	~Timer() {}

	std::string getFancyTime() const;

	void updateTimer();

	bool done() const { return finished; }

	SDL_Rect getRect() const { return rect; }

	void restart();
};