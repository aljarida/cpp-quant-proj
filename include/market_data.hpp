#pragma once

#include "candle.hpp"

#include <vector>
#include <string>

class MarketDataFeed
{
public:
	explicit MarketDataFeed(const std::string &filepath);
	bool next(Candle &candle);

private:
	std::vector<Candle> data_;
	size_t index_;

	void loadCSV(const std::string &filepath);
};
