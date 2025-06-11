#pragma once

#include "candle.hpp"

class Broker
{
public:
    virtual void buy(const Candle& c) = 0;
    virtual void sell(const Candle& c) = 0;
    virtual double get_cash() const = 0;
    virtual uint32_t get_position() const = 0;
    virtual ~Broker() = default;
};