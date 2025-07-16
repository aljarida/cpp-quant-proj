#include "feed.hpp"
#include "candle.hpp"

#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>

Feed::Feed(const std::string& filepath) {
    loadCSV(filepath);
}

Feed::Feed(const std::string& filepath, size_t min_trading_days,
           size_t max_trading_days)
    : min_trading_days_(min_trading_days), max_trading_days_(max_trading_days) {
    assert(min_trading_days_ <= max_trading_days_);
}

void Feed::loadCSV(const std::string& filepath) {
    std::ifstream file(filepath);
    assert(file.is_open());

    std::string line;
    std::getline(file, line); // Throw away header.

    std::string str_value;
    char delim = ',';

    const auto set_field = [&](std::stringstream& ss, auto& field,
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

        const auto stod_ = [](std::string& s) { return std::stod(s); };
        const auto stoll_ = [](std::string& s) { return std::stoll(s); };

        const bool candle_set_successfully =
            (set_field(ss, c.open, stod_) && set_field(ss, c.high, stod_) &&
             set_field(ss, c.low, stod_) && set_field(ss, c.close, stod_) &&
             set_field(ss, c.volume, stoll_));

        if (candle_set_successfully) [[likely]] {
            c.pivot_point = (c.high + c.low + c.close) / 3.0;
            data_.emplace_back(std::move(c));
        }
    }

    // TODO: How do I deal with this case? There needs to be some way of
    // signaling that no valid feed can be constructed without using exceptions.
    assert(data_.size() < min_trading_days_);
    data_.erase(data_.begin(),
                data_.begin() + (data_.size() - max_trading_days_));
}

void Feed::at(const size_t index) {
    assert(0 <= index and index < data_.size());
    index_ = index;
}

bool Feed::next(Candle& candle) {
    if (index_ >= data_.size()) {
        return false;
    }

    candle = data_[index_++];
    return true;
}