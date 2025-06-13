#pragma once

#include "broker.hpp"
#include "strategy.hpp"

#include <memory>
#include <vector>

class BuyAndHold : public Strategy {
  public:
    BuyAndHold(std::shared_ptr<Broker> b);
    void on_candle(const Candle &candle) override;

  private:
    std::shared_ptr<Broker> broker_;
};
