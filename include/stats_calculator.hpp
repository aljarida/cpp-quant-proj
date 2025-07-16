#pragma once

#include "stats.hpp"

#include <vector>

class StatsCalculator {
  public:
    StatsCalculator(const size_t reserve);
    void add_value(double value);
    Stats compute_all_stats();
    Stats compute_mean();

  private:
    std::vector<double> values_;
    double sum() const;
    double mean(const double sum) const;
    double standard_deviation(const double mean) const;
    std::tuple<double, double, double, double, double, double, double>
    min_max_and_percentiles();
};