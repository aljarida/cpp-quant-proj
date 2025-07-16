#pragma once

#include "constraints.hpp"

#include <cassert>
#include <deque>
#include <memory>
#include <numeric>
#include <vector>

template <IsBroker Broker>
class Donchian {
  public:
    struct ExtremumAndLifespan {
        double price;
        size_t lifespan;
    };

    Donchian(Broker&& broker, uint32_t period)
        : broker_(std::move(broker)), period_(period),
          buy_in_(broker_.get_cash()) {
    }

    void on_candle(const Candle& candle) {
        const bool ready = candles_.size() >= 20;
        if (ready) [[likely]]
            candles_.pop_front();

        candles_.push_back(candle);
        if (not extrema_initialized_) [[unlikely]]
            set_extrema(candle);

        if (ready) [[likely]] {
            const double max_yesterday = period_max_.price;
            const double min_yesterday = period_min_.price;

            const bool bullish_breakout = candle.close > max_yesterday;
            const bool bearish_breakout = candle.close < min_yesterday;
            if (bullish_breakout) {
                broker_.buy(candle);
            }

            if (bearish_breakout) {
                broker_.sell(candle);
            }
        }

        update_extrema(candle);
    }

    double profits() const {
        return (broker_.get_cash() +
                candles_.back().pivot_point * broker_.get_position()) -
               buy_in_;
    }

  private:
    const size_t period_ = 0;
    bool extrema_initialized_ = false;
    std::deque<Candle> candles_;
    Broker broker_;
    const double buy_in_;
    ExtremumAndLifespan period_max_;
    ExtremumAndLifespan period_min_;

    void set_extrema(const Candle& c) {
        period_max_ = {c.high, period_};
        period_min_ = {c.low, period_};
    }

    void update_extrema(const Candle& c) {
        period_max_.lifespan -= 1;
        period_min_.lifespan -= 1;

        if (period_max_.lifespan <= 0) {
            period_max_ = find_new_max();
        } else if (c.high >= period_max_.price) {
            period_max_.price = c.high;
            period_max_.lifespan = period_;
        }

        if (period_min_.lifespan <= 0) {
            period_min_ = find_new_min();
        } else if (c.low <= period_min_.price) {
            period_min_.price = c.low;
            period_min_.lifespan = period_;
        }
    }

    ExtremumAndLifespan find_new_max() {
        size_t index_of_max = candles_.size() - 1;
        double max = candles_[index_of_max].high;
        for (size_t i = index_of_max - 1; i >= 0; --i) {
            if (candles_[i].high > max) {
                max = candles_[i].high;
                index_of_max = i;
            }
        }

        return ExtremumAndLifespan{max, index_of_max + 1};
    }

    ExtremumAndLifespan find_new_min() {
        size_t index_of_min = candles_.size() - 1;
        double min = candles_[index_of_min].low;
        for (size_t i = index_of_min - 1; i >= 0; --i) {
            if (candles_[i].low < min) {
                min = candles_[i].low;
                index_of_min = i;
            }
        }

        return ExtremumAndLifespan{min, index_of_min + 1};
    }
};
