#pragma once
#include <string>
#include <vector>

struct AnalysisResults {
    std::vector<std::string> dates;
    std::vector<double> usd_rates;
    std::vector<double> ron_rates;
    std::vector<double> integrated_lacunarity;  // Combined lacunarity for both pairs
    std::vector<double> rolling_corr;
};