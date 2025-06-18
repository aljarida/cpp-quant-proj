#include "backtester.hpp"
#include "basic_broker.hpp"
#include "buy_and_hold.hpp"
#include "candle.hpp"
#include "feed.hpp"
#include "moving_average_cross.hpp"

#include <cassert>
#include <cmath>
#include <exception>
#include <filesystem>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

class Stats {
  public:
    double standard_deviation, mean;
    size_t total_stocks_analyzed;

    void print() {
        std::cout << "Standard deviation: " << standard_deviation << '\n';
        std::cout << "Mean return.: " << mean << std::endl;
    }
};

std::vector<std::string> get_stock_data_paths() {
    std::vector<std::string> res{};
    res.reserve(6000);
    for (const auto &entry : std::filesystem::directory_iterator(
             std::filesystem::path("../data/stocks"))) {

        assert(entry.path().extension() == ".csv");
        res.emplace_back(entry.path());
    }

    return res;
}

template <typename StrategyFactory>
double run_strategy_on_stock(const std::string &filepath,
                             const StrategyFactory make_strategy,
                             const double initial_cash = 1000) {

    auto broker_ptr = std::make_shared<BasicBroker>(initial_cash);
    auto strategy_ptr = make_strategy(broker_ptr);
    auto feed_ptr = std::make_shared<Feed>(filepath);

    Backtester backtester(broker_ptr, strategy_ptr, feed_ptr);
    backtester.run();

    return backtester.profits();
}

template <typename InitializeStrategy>
Stats run_strategy_on_all_stocks(const InitializeStrategy strategy,
                                 const double initial_cash = 1000) {
    std::vector<double> profits{};
    profits.reserve(6000);

    const std::vector<std::string> stock_paths = get_stock_data_paths();
    for (const std::string &path : stock_paths) {
        assert(path != "");
        profits.push_back(run_strategy_on_stock(path, strategy, initial_cash));
    }

    const double total_profits =
        std::accumulate(profits.begin(), profits.end(), 0.0);
    const double mean = total_profits / profits.size();
    const double rss = std::accumulate(
        profits.begin(), profits.end(), 0.0, [mean](double acc, double val) {
            return acc + (val - mean) * (val - mean);
        });
    const double standard_deviation = std::sqrt(rss / profits.size());

    return Stats{standard_deviation, mean, profits.size()};
}

int main() {
    auto init_mac = [](BasicBroker::Ptr broker) {
        return std::make_shared<MovingAverageCross<BasicBroker::Ptr>>(broker,
                                                                      50, 200);
    };

    Stats s1 = run_strategy_on_all_stocks(init_mac, 1000);
    s1.print();

    auto init_bah = [](BasicBroker::Ptr broker) {
        return std::make_shared<BuyAndHold<BasicBroker::Ptr>>(broker);
    };

    Stats s2 = run_strategy_on_all_stocks(init_bah, 1000);
    s2.print();

    return 0;
}