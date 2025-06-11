#pragma once

#include "broker.hpp"

class BasicBroker : public Broker {
  public:
    explicit BasicBroker(double cash);
    void buy(const Candle &c) override;
    void sell(const Candle &c) override;
    double get_cash() const override;
    uint32_t get_position() const override;

  private:
    double cash_;
    uint32_t position_;
};