#pragma once

#include "broker_cpt.hpp"

#include <cstdint>
#include <memory>
#include <numeric>
#include <vector>

template <IsBrokerPtr BrokerPtr>
class MovingAverageCross {
  public:
    MovingAverageCross(BrokerPtr broker, uint32_t short_period,
                       uint32_t long_period)
        : broker_(std::move(broker)), short_period_(short_period),
          long_period_(long_period) {
    }

    void on_candle(const Candle &candle) {
        closes_.push_back(candle.close);
        if (closes_.size() < long_period_)
            return;

        double short_ma = recent_moving_average(short_period_);
        double long_ma = recent_moving_average(long_period_);

        if (!position_open_ && short_ma > long_ma) {
            broker_->buy(candle);
            position_open_ = true;
        }

        if (position_open_ && short_ma < long_ma) {
            broker_->sell(candle);
            position_open_ = false;
        }
    }

  private:
    uint32_t short_period_ = 0;
    uint32_t long_period_ = 0;
    bool position_open_ = false;

    std::vector<double> closes_;

    BrokerPtr broker_;

    double recent_moving_average(uint32_t period) {
        std::vector<double>::iterator end = closes_.end();
        std::vector<double>::iterator begin = end - period;
        return std::accumulate(begin, end, 0.0) / std::distance(begin, end);
    }
};
