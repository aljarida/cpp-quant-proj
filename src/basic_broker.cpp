#include "basic_broker.hpp"

BasicBroker::BasicBroker(double cash) : cash_(cash) {}

void BasicBroker::buy(const Candle& c) {
    if (cash_ >= c.close) {
        cash_ -= c.close;
        position_ += 1;
    }
}

void BasicBroker::sell(const Candle& c) {
    while (position_ > 0) {
        cash_ += c.close;
        position_ -= 1;
    }
}

double BasicBroker::get_cash() const {
    return cash_;
}

uint32_t BasicBroker::get_position() const {
    return position_;
}