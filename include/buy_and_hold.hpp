#pragma once

#include "constraints.hpp"

#include <memory>
#include <vector>

template <IsBrokerPtr BrokerPtr>
class BuyAndHold {
  public:
    BuyAndHold(BrokerPtr broker) : broker_(std::move(broker)) {
    }

    void on_candle(const Candle &candle) {
        broker_->buy(candle);
    }

  private:
    BrokerPtr broker_;
};