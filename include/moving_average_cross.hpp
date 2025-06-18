#pragma once

#include "constraints.hpp"

#include <cassert>
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
        if (closes_.size() < long_period_) [[unlikely]] {
            return;
        }

        if (closes_.size() == long_period_) [[unlikely]] {
            initialize_moving_sums();
        } else [[likely]] {
            update_moving_sums();
        }

        double short_ma = recent_short_average();
        double long_ma = recent_long_average();

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
    const uint32_t short_period_ = 0;
    const uint32_t long_period_ = 0;
    bool position_open_ = false;

    uint64_t long_moving_sum_ = 0;
    uint64_t short_moving_sum_ = 0;

    std::vector<double> closes_;
    const BrokerPtr broker_;

    double recent_moving_sum(uint32_t period) {
        std::vector<double>::iterator end = closes_.end();
        std::vector<double>::iterator begin = end - period;
        return std::accumulate(begin, end, 0.0);
    }

    void initialize_moving_sums() {
        short_moving_sum_ = recent_moving_sum(short_period_);
        long_moving_sum_ = recent_moving_sum(long_period_);
    }

    void update_moving_sums() {
        const size_t first_long_period_elem_idx = closes_.size() - long_period_;
        const size_t first_short_period_elem_idx =
            closes_.size() - short_period_;
        const size_t lastest_elem_idx = closes_.size() - 1;

        assert(first_long_period_elem_idx > 0 and
               first_short_period_elem_idx > 0);

        long_moving_sum_ +=
            closes_[lastest_elem_idx] - closes_[first_long_period_elem_idx];
        short_moving_sum_ +=
            closes_[lastest_elem_idx] - closes_[first_short_period_elem_idx];
    }

    double recent_long_average() const {
        return long_moving_sum_ / long_period_;
    }

    double recent_short_average() const {
        return short_moving_sum_ / short_period_;
    }
};
