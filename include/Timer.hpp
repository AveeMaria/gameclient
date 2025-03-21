#pragma once

#include "Entity.hpp"
#include "TextRenderer.hpp"
#include <sstream>

class Timer {
private:
	uint32_t start_time;
	uint32_t curr_time;

	uint32_t seconds;//kok sekund (auto mnozi z 1000) 
	bool finished;
public:
	Timer();
	Timer(uint32_t sec);
	~Timer() {}

	std::string getFancyTime() const;

	void updateTimer();

	bool done() const { return finished; }

	void restart();
};