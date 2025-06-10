#pragma once

#include <string>
#include <cstdint>

struct Candle {
	std::string timestamp;
	double open;
	double high;
	double low;
	double close;
	uint32_t volume;
};
