#pragma once

#include <string>
#include <cstdint>

struct Candle {
	std::string timestamp;
	double open, high, low, close;
	uint32_t volume;
};
