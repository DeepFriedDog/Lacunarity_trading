#pragma once
#include <vector>
#include <string>
#include "types.hpp"

class DataLoader {
public:
    DataLoader() = default;
    AnalysisResults loadPriceData(const std::string& filepath);
}; 