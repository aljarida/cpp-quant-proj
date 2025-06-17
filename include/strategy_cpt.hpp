#pragma once

#include "candle.hpp"

#include <concepts>
#include <memory>

template <typename Strategy>
concept IsStrategy = requires(Strategy s, const Candle &c) {
    { s.on_candle(c) } -> std::same_as<void>;
};

template <typename StrategyPtr>
concept IsStrategyPtr =
    requires(StrategyPtr sptr) { requires IsStrategy<decltype(*sptr)>; };