#pragma once

#include "constraints.hpp"

#include <memory>
#include <vector>

template <IsBroker Broker>
class BuyAndHold {
  public:
    BuyAndHold(Broker&& broker)
        : broker_(std::move(broker)), buy_in_(broker_.get_cash()) {
    }

    void on_candle(const Candle& candle) {
        last_price_ = candle.pivot_point;
        if (bought_in_)
            return;

        broker_.buy(candle);
        bought_in_ = true;
    }

    double profits() const {
        return (broker_.get_cash() + last_price_ * broker_.get_position()) -
               buy_in_;
    }

  private:
    Broker broker_;
    bool bought_in_ = false;
    double last_price_;
    const double buy_in_;
};