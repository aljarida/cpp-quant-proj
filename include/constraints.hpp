#pragma once

#include "candle.hpp"

#include <concepts>
#include <memory>

template <typename Strategy>
concept IsStrategy = requires(Strategy s, const Candle &c) {
    { s.on_candle(c) } -> std::same_as<void>;
    { s.profits() } -> std::same_as<double>;
};

template <typename StrategyPtr>
concept IsStrategyPtr =
    requires(StrategyPtr s) { requires IsStrategy<decltype(*s)>; };

template <typename Broker>
concept IsBroker = requires(Broker b, const Broker cb, const Candle &c) {
    { b.buy(c) } -> std::same_as<void>;
    { b.sell(c) } -> std::same_as<void>;
    { cb.get_cash() } -> std::same_as<double>;
    { cb.get_position() } -> std::same_as<uint32_t>;
    { b.reset() } -> std::same_as<void>;
};

template <typename BrokerPtr>
concept IsBrokerPtr =
    requires(BrokerPtr b) { requires IsBroker<decltype(*b)>; };