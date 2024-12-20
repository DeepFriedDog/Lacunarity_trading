#include "data_loader.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>

// Implement loadPriceData method
AnalysisResults DataLoader::loadPriceData(const std::string& filepath) {
    AnalysisResults results;
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filepath);
    }

    std::string line;
    // Assuming CSV format: date,usd_rate,ron_rate
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string date;
        double usd_rate, ron_rate;
        if (!(iss >> date >> usd_rate >> ron_rate)) {
            continue; // Skip malformed lines
        }
        results.dates.push_back(date);
        results.usd_rates.push_back(usd_rate);
        results.ron_rates.push_back(ron_rate);
    }

    return results;
} 