#include "basic_broker.hpp"
#include "candle.hpp"

#include <cstdint>

BasicBroker::BasicBroker(double cash) : cash_(cash), position_(0) {
}

void BasicBroker::buy(const Candle &c) {
    const double price = get_buy_price(c);
    while (cash_ >= price) {
        cash_ -= price;
        position_ += 1;
    }
}

void BasicBroker::sell(const Candle &c) {
    const double price = get_sell_price(c);
    while (position_ > 0) {
        cash_ += price;
        position_ -= 1;
    }
}

double BasicBroker::get_cash() const {
    return cash_;
}

uint32_t BasicBroker::get_position() const {
    return position_;
}

double BasicBroker::get_buy_price(const Candle &c) const {
    const double slippage = simulate_slippage(c);
    return c.pivot_point + (slippage > 0 ? +slippage : -slippage);
}

double BasicBroker::get_sell_price(const Candle &c) const {
    const double slippage = simulate_slippage(c);
    return c.pivot_point + (slippage > 0 ? -slippage : +slippage);
}

double BasicBroker::simulate_slippage(const Candle &c) const {
    const double slippage_constant = 0.1;
    return slippage_constant * (c.high - c.low);
}

void BasicBroker::reset() {
    cash_ = 0;
    position_ = 0;
}