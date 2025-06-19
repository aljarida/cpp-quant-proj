#include "stats_calculator.hpp"
#include "stats.hpp"

#include <algorithm>
#include <cmath>
#include <numeric>

StatsCalculator::StatsCalculator(const size_t reserve) {
    values_.reserve(reserve);
}

void StatsCalculator::add_value(double value) {
    values_.push_back(value);
}

Stats StatsCalculator::compute_all_stats() {
    if (values_.empty())
        return Stats{};

    Stats s;
    s.sum = sum();
    s.mean = mean(s.sum);
    s.standard_deviation = standard_deviation(s.mean);
    std::tie(s.minimum, s.twenty_fifth, s.median, s.seventy_fifth, s.maximum) =
        min_25th_med_75th_max();

    return s;
}

Stats StatsCalculator::compute_mean() {
    if (values_.empty())
        return Stats{};

    Stats s;
    s.sum = sum();
    return s;
}

double StatsCalculator::sum() const {
    return std::accumulate(values_.begin(), values_.end(), 0.0);
}

double StatsCalculator::mean(const double sum) const {
    return sum / values_.size();
}

double StatsCalculator::standard_deviation(const double mean) const {
    const double rss =
        std::accumulate(values_.begin(), values_.end(), 0.0,
                        [mean](const double acc, const double val) {
                            return acc + (val - mean) * (val - mean);
                        });
    return std::sqrt(rss / values_.size());
}

std::tuple<double, double, double, double, double>
StatsCalculator::min_25th_med_75th_max() {
    if (values_.empty())
        return {0.0, 0.0, 0.0, 0.0, 0.0};

    std::sort(values_.begin(), values_.end());
    const double min = values_.front();
    const double max = values_.back();

    // Use linear interpolation between closest ranks.
    auto get_percentile = [&](double p) -> double {
        double x = p * (values_.size() - 1);
        size_t floor_x = static_cast<size_t>(std::floor(x));
        size_t ceil_x = static_cast<size_t>(std::ceil(x));
        double x_mod_1 = x - floor_x;
        return values_[x] + x_mod_1 * (values_[ceil_x] - values_[floor_x]);
    };

    const double twenty_fifth = get_percentile(0.25);
    const double median = get_percentile(0.5);
    const double seventy_fifth = get_percentile(0.75);

    return {min, twenty_fifth, median, seventy_fifth, max};
}