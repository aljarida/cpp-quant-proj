#include "backtester.hpp"
#include "basic_broker.hpp"
#include "buy_and_hold.hpp"
#include "candle.hpp"
#include "donchian.hpp"
#include "feed.hpp"
#include "moving_average_cross.hpp"
#include "stats.hpp"
#include "stats_calculator.hpp"

#include <cassert>
#include <cmath>
#include <exception>
#include <filesystem>
#include <iostream>
#include <numeric>
#include <optional>
#include <string>
#include <vector>

const size_t NUM_STOCKS{6000};

std::vector<std::string> get_stock_data_paths() {
    std::vector<std::string> res{};
    res.reserve(NUM_STOCKS);

    for (const auto& entry : std::filesystem::directory_iterator(
             std::filesystem::path("../data/stocks"))) {

        assert(entry.path().extension() == ".csv");
        res.emplace_back(entry.path());
    }

    return res;
}

template <typename StrategyFactory>
double run_strategy_on_stock(const std::string& filepath,
                             const StrategyFactory make_strategy,
                             const double initial_cash = 1000) {

    BasicBroker broker(initial_cash);

    auto strategy = make_strategy(std::move(broker));
    Feed feed(filepath);

    Backtester backtester(std::move(strategy), std::move(feed));
    backtester.run();

    return backtester.profits();
}

template <typename InitializeStrategy>
Stats run_strategy_on_all_stocks(const InitializeStrategy strategy,
                                 const std::vector<std::string>& stock_paths,
                                 const double initial_cash = 1000) {
    StatsCalculator sc(NUM_STOCKS);

    for (const std::string& path : stock_paths) {
        assert(path != "");
        sc.add_value(run_strategy_on_stock(path, strategy, initial_cash));
    }

    Stats s = sc.compute_all_stats();

    return s;
}

void print_stats(Stats s) {
    std::cout << "Mean: " << s.mean << std::endl;
    std::cout << "Std.: " << s.standard_deviation << std::endl;
    std::cout << "Min.: " << s.minimum << std::endl;
    std::cout << "10p.: " << s.tenth << std::endl;
    std::cout << "25p.: " << s.twenty_fifth << std::endl;
    std::cout << "Med.: " << s.median << std::endl;
    std::cout << "75p.: " << s.seventy_fifth << std::endl;
    std::cout << "90p.: " << s.ninetieth << std::endl;
    std::cout << "Max.: " << s.maximum << std::endl;
}

int main() {
    const auto& stock_paths = get_stock_data_paths();

    auto init_mac = [](BasicBroker&& broker) {
        return MovingAverageCross(std::move(broker), 50, 200);
    };
    std::cout << "Testing Moving Average Cross" << std::endl;
    Stats s1 = run_strategy_on_all_stocks(init_mac, stock_paths, 1000);
    print_stats(s1);

    auto init_bah = [](BasicBroker&& broker) {
        return BuyAndHold(std::move(broker));
    };
    std::cout << "Testing Buy And Hold" << std::endl;
    Stats s2 = run_strategy_on_all_stocks(init_bah, stock_paths, 1000);
    print_stats(s2);

    auto init_donchian = [](BasicBroker&& broker) {
        return Donchian(std::move(broker), 30);
    };
    std::cout << "Testing Donchian" << std::endl;
    Stats s3 = run_strategy_on_all_stocks(init_donchian, stock_paths, 1000);
    print_stats(s3);

    return 0;
}