#pragma once

#include <memory>

#include "broker.hpp"
#include "candle.hpp"
#include "market_data.hpp"
#include "strategy.hpp"

class Backtester {
  public:
    Backtester(std::shared_ptr<Broker> broker, std::shared_ptr<Strategy> strategy,
               std::shared_ptr<MarketDataFeed> feed);

    void run();
    double profits() const;

  private:
    std::shared_ptr<Broker> broker_;
    std::shared_ptr<Strategy> strategy_;
    std::shared_ptr<MarketDataFeed> feed_;

    double last_close_;
    double initial_buy_in_;
};