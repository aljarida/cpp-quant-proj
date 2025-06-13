#pragma once

#include "candle.hpp"

#include <string>
#include <vector>

class MarketDataFeed {
  public:
    explicit MarketDataFeed(const std::string &filepath);
    bool next(Candle &candle);
    void at(const size_t index);

  private:
    std::vector<Candle> data_;
    size_t index_;
    void loadCSV(const std::string &filepath);
};
