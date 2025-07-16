#pragma once

#include "candle.hpp"

#include <string>
#include <vector>

class Feed {
  public:
    Feed(const std::string& filepath);
    Feed(const std::string& filepath, size_t min_trading_window,
         size_t max_trading_window);
    bool next(Candle& candle);
    void at(const size_t index);

  private:
    void loadCSV(const std::string& filepath);
    std::vector<Candle> data_;
    size_t index_ = 0;
    size_t max_trading_days_ = 0;
    size_t min_trading_days_ = 0;
};