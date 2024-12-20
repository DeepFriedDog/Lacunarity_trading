#include "plotter.hpp"
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <numeric>
#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

Plotter::Plotter() {}

void Plotter::plot(const AnalysisResults& results) {
    std::cout << "\nPlotting data ranges:" << std::endl;
    std::cout << "Total dates: " << results.dates.size() << std::endl;
    std::cout << "Date range: " << results.dates.front() << " to " << results.dates.back() << std::endl;
    std::cout << "Lacunarity values: " << results.integrated_lacunarity.size() << std::endl;
    std::cout << "Correlation values: " << results.rolling_corr.size() << std::endl;

    try {
        saveDataToJSON(results);
        std::cout << "Data saved to JSON successfully" << std::endl;
        
        generatePythonScript();
        std::cout << "Python script generated successfully" << std::endl;
        
        executePythonScript();
        std::cout << "Python script executed successfully" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error during plotting: " << e.what() << std::endl;
        throw;
    }
}

void Plotter::saveDataToCSV(const AnalysisResults& results) {
    std::ofstream file(DATA_FILE);
    if (!file) {
        throw std::runtime_error("Failed to create data file");
    }

    // Write headers
    file << "date,integrated_lacunarity,rolling_corr\n";

    // Calculate the starting index to align data
    size_t start_offset = results.dates.size() - results.integrated_lacunarity.size();
    
    // Write aligned data
    for (size_t i = start_offset; i < results.dates.size(); ++i) {
        size_t lac_idx = i - start_offset;
        file << results.dates[i] << ",";
        file << results.integrated_lacunarity[lac_idx] << ",";
        file << results.rolling_corr[lac_idx] << "\n";
    }
}

void Plotter::saveDataToJSON(const AnalysisResults& results) {
    json output;
    
    // Calculate the starting indices to align all data
    size_t lac_offset = results.dates.size() - results.integrated_lacunarity.size();
    size_t corr_offset = results.dates.size() - results.rolling_corr.size();
    size_t max_offset = std::max(lac_offset, corr_offset);
    
    // Create aligned arrays
    std::vector<std::string> aligned_dates;
    std::vector<double> aligned_lacunarity;
    std::vector<double> aligned_correlation;
    
    size_t final_size = results.dates.size() - max_offset;
    aligned_dates.reserve(final_size);
    aligned_lacunarity.reserve(final_size);
    aligned_correlation.reserve(final_size);
    
    // Align all data series to the same length
    for (size_t i = max_offset; i < results.dates.size(); ++i) {
        aligned_dates.push_back(results.dates[i]);
        aligned_lacunarity.push_back(results.integrated_lacunarity[i - lac_offset]);
        aligned_correlation.push_back(results.rolling_corr[i - corr_offset]);
    }
    
    // Add aligned data to JSON object
    output["dates"] = aligned_dates;
    output["integrated_lacunarity"] = aligned_lacunarity;
    output["rolling_corr"] = aligned_correlation;
    output["start_date"] = aligned_dates.front();
    output["end_date"] = aligned_dates.back();
    
    // Write to file
    std::ofstream file("exchange_data.json");
    if (!file) {
        throw std::runtime_error("Failed to create JSON file");
    }
    file << output.dump(4);
    
    std::cout << "Aligned data sizes - Dates: " << aligned_dates.size() 
              << ", Lacunarity: " << aligned_lacunarity.size()
              << ", Correlation: " << aligned_correlation.size() << std::endl;
}

void Plotter::generatePythonScript() {
    std::ofstream file(SCRIPT_FILE);
    if (!file) {
        throw std::runtime_error("Failed to create Python script");
    }

    file << R"(
import pandas as pd
import matplotlib.pyplot as plt
import json

# Read data from JSON
with open('exchange_data.json', 'r') as f:
    data = json.load(f)

# Convert to DataFrame
df = pd.DataFrame({
    'date': pd.to_datetime(data['dates']),
    'integrated_lacunarity': data['integrated_lacunarity'],
    'rolling_corr': data['rolling_corr']
})

# Calculate smoothed averages
window_size = 30
df['smoothed_lacunarity'] = df['integrated_lacunarity'].rolling(window=window_size, min_periods=1, center=True).mean()
df['smoothed_correlation'] = df['rolling_corr'].rolling(window=window_size, min_periods=1, center=True).mean()

# Set style and create figure with 4 subplots
plt.style.use('seaborn-v0_8-whitegrid')
fig, (ax1, ax2, ax3, ax4) = plt.subplots(4, 1, figsize=(12, 20))

# Plot 1: Correlation Values
ax1.plot(df['date'], df['rolling_corr'], '-', color='blue', label='Correlation')
ax1.set_title('Rolling Correlation Over Time')
ax1.set_xlabel('Time')
ax1.set_ylabel('Correlation Value')
ax1.grid(True, alpha=0.3)
ax1.legend(loc='upper right')

# Plot 2: Integrated Lacunarity
ax2.plot(df['date'], df['integrated_lacunarity'], '-', color='green', label='Lacunarity (Power Law)')
ax2.set_title('Lacunarity Coefficient Over Time')
ax2.set_xlabel('Time')
ax2.set_ylabel('Lacunarity (Power Law)')
ax2.grid(True, alpha=0.3)
ax2.legend(loc='upper right')

# Plot 3: Overlap of both metrics
ax3.plot(df['date'], df['rolling_corr'], '-', color='blue', label='Correlation')
ax3.plot(df['date'], df['integrated_lacunarity'], '-', color='green', label='Lacunarity (Power Law)')
ax3.set_title('Overlap of Correlation and Lacunarity Coefficients Over Time')
ax3.set_xlabel('Time')
ax3.set_ylabel('Value')
ax3.grid(True, alpha=0.3)
ax3.legend(loc='upper right')

# Plot 4: Smoothed averages
ax4.plot(df['date'], df['smoothed_correlation'], '-', color='blue', linewidth=2, label='Smoothed Correlation')
ax4.plot(df['date'], df['smoothed_lacunarity'], '-', color='green', linewidth=2, label='Smoothed Lacunarity')
ax4.set_title('Smoothed Averages (30-day window)')
ax4.set_xlabel('Time')
ax4.set_ylabel('Value')
ax4.grid(True, alpha=0.3)
ax4.legend(loc='upper right')

# Rotate x-axis labels for better readability
for ax in [ax1, ax2, ax3, ax4]:
    ax.tick_params(axis='x', rotation=45)

# Adjust layout and save
plt.tight_layout()
plt.savefig('exchange_analysis.png', dpi=300, bbox_inches='tight')
plt.close()
)";
}

void Plotter::executePythonScript() {
    #ifdef _WIN32
        int result = system("python generate_plots.py");
    #else
        int result = system("python3 generate_plots.py");
    #endif

    if (result != 0) {
        throw std::runtime_error("Failed to execute Python script");
    }
}

std::vector<double> calculateLacunarity(const std::vector<double>& data) {
    std::vector<double> lacunarity;
    const int window_size = 30; // Fixed window size for time series analysis
    
    for (size_t i = 0; i + window_size <= data.size(); ++i) {
        // Extract window
        std::vector<double> window(data.begin() + i, data.begin() + i + window_size);
        
        // Calculate mean and variance for the window
        double mean = std::accumulate(window.begin(), window.end(), 0.0) / window_size;
        
        // Calculate box mass (density) according to GBL formula
        double box_mass = 0.0;
        for (const auto& val : window) {
            box_mass += std::abs(val - mean);
        }
        box_mass /= window_size;
        
        // Calculate lacunarity as per the paper's formula
        double variance = 0.0;
        for (const auto& val : window) {
            double diff = std::abs(val - mean) - box_mass;
            variance += diff * diff;
        }
        variance /= window_size;
        
        // Final lacunarity value for this window
        double lac = (box_mass != 0) ? variance / (box_mass * box_mass) : 0.0;
        lacunarity.push_back(lac);
    }
    
    return lacunarity;
}

std::vector<double> calculateRollingCorrelation(const std::vector<double>& usd_rates, 
                                              const std::vector<double>& ron_rates,
                                              int window_size = 30) {
    std::vector<double> rolling_corr;
    
    for (size_t i = 0; i + window_size <= usd_rates.size(); ++i) {
        double sum_x = 0, sum_y = 0, sum_xy = 0;
        double sum_x2 = 0, sum_y2 = 0;
        
        for (int j = 0; j < window_size; ++j) {
            double x = usd_rates[i + j];
            double y = ron_rates[i + j];
            
            sum_x += x;
            sum_y += y;
            sum_xy += x * y;
            sum_x2 += x * x;
            sum_y2 += y * y;
        }
        
        double n = window_size;
        double numerator = n * sum_xy - sum_x * sum_y;
        double denominator = std::sqrt((n * sum_x2 - sum_x * sum_x) * 
                                    (n * sum_y2 - sum_y * sum_y));
        
        double correlation = (denominator != 0) ? numerator / denominator : 0;
        rolling_corr.push_back(correlation);
    }
    
    return rolling_corr;
}

