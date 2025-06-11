#pragma once

#include "candle.hpp"
#include "broker.hpp"

#include <memory>

class Strategy
{
public:
    virtual void on_candle(const Candle& candle) = 0;
    virtual ~Strategy() = default;
};