#include "backtester.hpp"
#include "basic_broker.hpp"
#include "buy_and_hold.hpp"
#include "candle.hpp"
#include "market_data.hpp"
#include "moving_average_cross.hpp"

#include <cassert>
#include <cmath>
#include <exception>
#include <filesystem>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

struct Stats {
    double standard_deviation, mean;
    size_t total_stocks;

    void print() {
        std::cout << "std.: " << standard_deviation << std::endl;
        std::cout << "mean.: " << mean << std::endl;
    }
};

std::vector<std::string> get_stock_data_paths() {
    const std::string data_dir = "../data/stocks/";

    std::vector<std::string> res{};
    res.reserve(6000);
    for (const auto &entry : std::filesystem::directory_iterator(data_dir)) {
        if (entry.path().extension() == ".csv") {
            std::string filepath = entry.path();
            res.push_back(filepath);
        }
    }

    return res;
}

template <typename InitializeStrategy>
double run_strategy_on_stock(const std::string &filepath, InitializeStrategy strategy,
                             double initial_cash = 1000) {

    auto broker = std::make_shared<BasicBroker>(initial_cash);
    auto feed = std::make_shared<MarketDataFeed>(filepath);

    Backtester backtester(broker, strategy(broker), feed);
    backtester.run();

    return backtester.profits();
}

template <typename InitializeStrategy>
Stats run_strategy_on_all_stocks(InitializeStrategy strategy, double initial_cash = 1000) {
    std::vector<double> profits(6000);

    const std::vector<std::string> stock_paths = get_stock_data_paths();
    for (const std::string &path : stock_paths) {
        assert(path != "");
        profits.push_back(run_strategy_on_stock(path, strategy, initial_cash));
    }

    const double total_profits = std::accumulate(profits.begin(), profits.end(), 0.0);
    const double mean = total_profits / profits.size();
    const double rss =
        std::accumulate(profits.begin(), profits.end(), 0.0, [mean](double acc, double val) {
            return acc + (val - mean) * (val - mean);
        });
    const double standard_deviation = std::sqrt(rss / profits.size());

    return Stats{standard_deviation, mean, profits.size()};
}

int main() {
    auto init_mac = [](std::shared_ptr<BasicBroker> broker) {
        return std::make_shared<MovingAverageCross>(broker, 50, 200);
    };

    Stats s = run_strategy_on_all_stocks(init_mac, 1000);
    s.print();

    auto init_bah = [](std::shared_ptr<BasicBroker> broker) {
        return std::make_shared<BuyAndHold>(broker);
    };

    s = run_strategy_on_all_stocks(init_bah, 1000);
    s.print();

    return 0;
}