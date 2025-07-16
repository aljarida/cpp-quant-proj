#pragma once

#include "candle.hpp"

#include <cstdint>
#include <memory>

class BasicBroker {
  public:
    using Ptr = std::shared_ptr<BasicBroker>;

    BasicBroker(double cash);
    void buy(const Candle& c);
    void sell(const Candle& c);
    double get_cash() const;
    uint32_t get_position() const;
    void reset();

  private:
    double get_buy_price(const Candle& c) const;
    double get_sell_price(const Candle& c) const;
    double simulate_slippage(const Candle& c) const;
    double cash_;
    uint32_t position_;
};