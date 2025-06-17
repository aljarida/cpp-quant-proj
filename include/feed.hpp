#pragma once

#include "candle.hpp"

#include <string>
#include <vector>

class Feed {
  public:
    Feed(const std::string &filepath);
    bool next(Candle &candle);
    void at(const size_t index);

  private:
    void loadCSV(const std::string &filepath);
    size_t index_;
    std::vector<Candle> data_;
};