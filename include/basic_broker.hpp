#pragma once

#include "candle.hpp"

#include <cstdint>

class BasicBroker {
  public:
    BasicBroker(double cash);
    void buy(const Candle &c);
    void sell(const Candle &c);
    double get_cash() const;
    uint32_t get_position() const;
    void reset();

  private:
    double cash_;
    uint32_t position_;
};