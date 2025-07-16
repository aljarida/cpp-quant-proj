#pragma once

#include <memory>

#include "candle.hpp"
#include "constraints.hpp"
#include "feed.hpp"

template <IsStrategy Strategy>
class Backtester {
  public:
    Backtester(Strategy&& strategy, Feed&& feed)
        : strategy_(std::move(strategy)), feed_(std::move(feed)) {
    }

    void run() {
        Candle c;
        feed_.at(0);
        while (feed_.next(c)) {
            strategy_.on_candle(c);
        }
    }

    double profits() const {
        return strategy_.profits();
    }

  private:
    Strategy strategy_;
    Feed feed_;
};