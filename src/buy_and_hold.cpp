#include "buy_and_hold.hpp"
#include "broker.hpp"
#include "strategy.hpp"

#include <memory>
#include <vector>

BuyAndHold::BuyAndHold(std::shared_ptr<Broker> b) : broker_(std::move(b)) {}

void BuyAndHold::on_candle(const Candle &candle) { broker_->buy(candle); }