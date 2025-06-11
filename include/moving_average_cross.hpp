#pragma once

#include "strategy.hpp"
#include "broker.hpp"

#include <cstdint>
#include <vector>
#include <memory>

class MovingAverageCross : public Strategy {
public:
    MovingAverageCross(uint32_t short_period, uint32_t long_period, std::shared_ptr<Broker> b);
    void on_candle(const Candle& candle) override;

private:
    uint32_t short_period_;
    uint32_t long_period_;
    bool position_open_ = false;
    
    std::vector<double> closes_;

    std::shared_ptr<Broker> broker_;

    double recent_moving_average(uint32_t period);
};
