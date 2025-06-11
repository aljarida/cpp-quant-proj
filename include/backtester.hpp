#pragma once

#include <memory>

#include "strategy.hpp"
#include "market_data.hpp"
#include "candle.hpp"
#include "broker.hpp"

class Backtester {
public:
    Backtester(
        std::shared_ptr<Broker> broker,
        std::shared_ptr<Strategy> strategy,
        std::shared_ptr<MarketDataFeed> feed
    );

    void run();
    double profits() const;

private:
    std::shared_ptr<Broker> broker_;
    std::shared_ptr<Strategy> strategy_;
    std::shared_ptr<MarketDataFeed> feed_;

    double last_close_;
};