#include "backtester.hpp"
#include "moving_average_cross.hpp"
#include "market_data.hpp"
#include "basic_broker.hpp"

#include <iostream>

int main() {
    auto broker = std::make_shared<BasicBroker>(1000.0);
    auto strategy = std::make_shared<MovingAverageCross>(5, 20, broker);
    auto feed = std::make_shared<MarketDataFeed>("../data/IBM.csv");

    Backtester backtester(broker, strategy, feed);
    backtester.run();

    const double profit = broker->get_cash();
    std::cout << "Total profit was: " << backtester.profits() << std::endl;

    return 0;
}