#pragma once

#include "broker.hpp"
#include "strategy.hpp"

#include <cstdint>
#include <memory>
#include <vector>

class MovingAverageCross : public Strategy {
  public:
    MovingAverageCross(std::shared_ptr<Broker> b, uint32_t short_period, uint32_t long_period);
    void on_candle(const Candle &candle) override;

  private:
    uint32_t short_period_ = 0;
    uint32_t long_period_ = 0;
    bool position_open_ = false;

    std::vector<double> closes_;

    std::shared_ptr<Broker> broker_;

    double recent_moving_average(uint32_t period);
};
