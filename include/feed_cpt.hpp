#pragma once

#include "candle.hpp"

#include <concepts>
#include <string>
#include <vector>

template <typename Feed>
concept IsFeed =
    requires(Feed f, const std::string &path, Candle &c, size_t idx) {
        { f.next(c) } -> std::same_as<bool>;
        { f.at(idx) } -> std::same_as<void>;
    };

template <typename FeedPtr>
concept IsFeedPtr =
    requires(FeedPtr fptr) { requires IsFeed<decltype(*fptr)>; };
