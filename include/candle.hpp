#pragma once

#include <cstdint>
#include <string>

struct Candle {
    std::string timestamp;
    double open, high, low, close;
    uint32_t volume;
};
