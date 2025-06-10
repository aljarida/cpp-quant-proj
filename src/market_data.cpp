#include "market_data.hpp"

#include <fstream>
#include <sstream>
#include <iostream>

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

	auto set_field = [&](auto& field, auto convert) {
		std::getline(ss, str_value, delim);
		field = convert(str_value)
	}

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        Candle c;


        std::getline(ss, c.timestamp, delim);

		set_field(c.open, std::stod);
		set_field(c.high, std::stod);
		set_field(c.low, std::stod);
		set_field(c.close, std::stod);
		
		set_field(c.volume, std::stoi);

        data_.push_back(c);
    }
}

bool MarketDataFeed::next(Candle& candle) {
    if (index_ >= data_.size()) return false;
    candle = data_[index_++];
    return true;
}
