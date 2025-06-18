#pragma once

#include <memory>

#include "candle.hpp"
#include "constraints.hpp"
#include "feed.hpp"

template <IsBrokerPtr BrokerPtr, IsStrategyPtr StrategyPtr>
class Backtester {
  public:
    Backtester(BrokerPtr broker, StrategyPtr strategy,
               std::shared_ptr<Feed> feed)
        : broker_(std::move(broker)), strategy_(std::move(strategy)),
          feed_(std::move(feed)), initial_buy_in_(broker_->get_cash()) {
    }

    void run() {
        Candle c;
        feed_->at(0);
        while (feed_->next(c)) {
            strategy_->on_candle(c);
        }
        last_close_ = c.close;
    }

    double profits() const {
        const double current_net_worth =
            broker_->get_cash() + (last_close_ * broker_->get_position());
        return current_net_worth - initial_buy_in_;
    }

  private:
    BrokerPtr broker_;
    StrategyPtr strategy_;
    std::shared_ptr<Feed> feed_;

    double last_close_;
    double initial_buy_in_;
};