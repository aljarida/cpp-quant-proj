#include "feed.hpp"
#include "candle.hpp"

#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>

Feed::Feed(const std::string &filepath) : index_(0) {
    loadCSV(filepath);
}

void Feed::loadCSV(const std::string &filepath) {
    std::ifstream file(filepath);
    assert(file.is_open());

    std::string line;
    std::getline(file, line); // Throw away header.

    std::string str_value;
    char delim = ',';

    const auto set_field = [&](std::stringstream &ss, auto &field,
                               const auto convert) -> bool {
        std::getline(ss, str_value, delim);
        if (str_value == "")
            return false;

        field = convert(str_value);
        return true;
    };

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        Candle c;

        std::getline(ss, c.timestamp, delim);

        const auto stod_ = [](std::string &s) { return std::stod(s); };
        const auto stoll_ = [](std::string &s) { return std::stoll(s); };

        const bool candle_set_successfully =
            (set_field(ss, c.open, stod_) && set_field(ss, c.high, stod_) &&
             set_field(ss, c.low, stod_) && set_field(ss, c.close, stod_) &&
             set_field(ss, c.volume, stoll_));

        if (candle_set_successfully) {
            c.pivot_point = (c.high + c.low + c.close) / 3.0;
            data_.emplace_back(std::move(c));
        }
    }
}

void Feed::at(const size_t index) {
    assert(0 <= index and index < data_.size());
    index_ = index;
}

bool Feed::next(Candle &candle) {
    if (index_ >= data_.size()) {
        return false;
    }

    candle = data_[index_++];
    return true;
}