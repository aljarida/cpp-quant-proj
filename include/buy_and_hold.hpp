#pragma once

#include "broker_cpt.hpp"

#include <memory>
#include <vector>

template <IsBrokerPtr BrokerPtr>
class BuyAndHold {
  public:
    BuyAndHold(BrokerPtr bptr) : broker_(std::move(bptr)) {
    }

    void on_candle(const Candle &candle) {
        broker_->buy(candle);
    }

  private:
    BrokerPtr broker_;
};