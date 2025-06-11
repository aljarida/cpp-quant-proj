#pragma once

#include "broker.hpp"
#include "candle.hpp"

#include <memory>

class Strategy {
  public:
    virtual void on_candle(const Candle &candle) = 0;
    virtual ~Strategy() = default;
};