#include "backtester.hpp"
#include "basic_broker.hpp"
#include "market_data.hpp"
#include "moving_average_cross.hpp"

#include <iostream>

int main() {
    auto broker = std::make_shared<BasicBroker>(10000.0);
    auto strategy = std::make_shared<MovingAverageCross>(50, 200, broker);
    auto feed = std::make_shared<MarketDataFeed>("../data/stocks/BNGO.csv");

    Backtester backtester(broker, strategy, feed);
    backtester.run();

    std::cout << "Total profit was: " << backtester.profits() << std::endl;

    return 0;
}