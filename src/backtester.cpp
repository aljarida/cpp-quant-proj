#include "backtester.hpp"

#include <iostream>

Backtester::Backtester(std::shared_ptr<Broker> broker, std::shared_ptr<Strategy> strategy,
                       std::shared_ptr<MarketDataFeed> feed)
    : broker_(std::move(broker)), strategy_(std::move(strategy)), feed_(std::move(feed)),
      initial_buy_in_(broker_->get_cash()) {}

void Backtester::run() {
    Candle c;

    feed_->at(0);
    while (feed_->next(c)) {
        strategy_->on_candle(c);
    }

    last_close_ = c.close;
}

double Backtester::profits() const {
    const double current_net_worth = broker_->get_cash() + (last_close_ * broker_->get_position());
    return current_net_worth - initial_buy_in_;
}