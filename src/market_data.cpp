#include "market_data.hpp"

#include <fstream>
#include <sstream>
#include <iostream>
#include <cassert>

MarketDataFeed::MarketDataFeed(const std::string& filepath) : index_(0) {
	loadCSV(filepath);
}

void MarketDataFeed::loadCSV(const std::string& filepath) {
	std::ifstream file(filepath);
	assert(file.is_open());

	std::string line;
    std::getline(file, line); // Throw away header.


	std::string str_value;
	char delim = ',';

	const auto set_field = [&](std::stringstream& ss, auto& field, const auto convert) {
		std::getline(ss, str_value, delim);
		field = convert(str_value);
	};

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        Candle c;


        std::getline(ss, c.timestamp, delim);

		const auto stod_ = [](std::string& s) { return std::stod(s); };
		const auto stoi_ = [](std::string& s) { return std::stoi(s); };

		set_field(ss, c.open, stod_);
		set_field(ss, c.high, stod_);
		set_field(ss, c.low, stod_);
		set_field(ss, c.close, stod_);
		
		set_field(ss, c.volume, stoi_);

        data_.push_back(c);
    }
}

bool MarketDataFeed::next(Candle& candle) {
    if (index_ >= data_.size()) {
		return false;
	}

    candle = data_[index_++];
    return true;
}
