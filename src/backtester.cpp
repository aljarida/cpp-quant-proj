#include "backtester.hpp"

#include <iostream>

Backtester::Backtester(
    std::shared_ptr<Broker> broker,
    std::shared_ptr<Strategy> strategy,
    std::shared_ptr<MarketDataFeed> feed
) : broker_(std::move(broker)), strategy_(std::move(strategy)), feed_(std::move(feed)) {}

void Backtester::run() {
    Candle c;
    while (feed_->next(c)) {
        strategy_->on_candle(c);
    }

    last_close_ = c.close;
}

double Backtester::profits() const {
    return broker_->get_cash() + (last_close_ * broker_->get_position());
}