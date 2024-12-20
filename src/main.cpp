#include "core/analyzer.hpp"
#include "utils/ecb_fetcher.hpp"
#include "utils/plotter.hpp"
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

// Helper function to convert ECBFetcher::DataVector to std::vector<double>
std::vector<double> convertToVector(const ECBFetcher::DataVector& data) {
    std::vector<double> result;
    result.reserve(data.size());
    for (const auto& point : data) {
        result.push_back(point.rate);  // Changed from point.value to point.rate
    }
    return result;
}

int main() {
    try {
        std::cout << "Starting program..." << std::endl;
        
        std::cout << "Initializing ECB fetcher..." << std::endl;
        ECBFetcher fetcher;
        
        // User input for date range
        std::string start_date, end_date;
        std::cout << "Enter start date (YYYY-MM-DD): ";
        std::cin >> start_date;
        std::cout << "Enter end date (YYYY-MM-DD): ";
        std::cin >> end_date;
        
        std::cout << "Loading exchange rate data..." << std::endl;
        auto data = fetcher.fetchData(start_date, end_date);
        
        // Add these debug lines
        std::cout << "USD data range: " << data.usd_data.front().date << " to " << data.usd_data.back().date << std::endl;
        std::cout << "RON data range: " << data.ron_data.front().date << " to " << data.ron_data.back().date << std::endl;
        std::cout << "Loaded " << data.usd_data.size() + data.ron_data.size() << " data points" << std::endl;
        
        // Use actual dates from the fetched data
        std::vector<std::string> dates;
        dates.reserve(data.usd_data.size());
        for (const auto& point : data.usd_data) {
            dates.push_back(point.date);
        }
        
        std::cout << "Date range: " << dates.front() << " to " << dates.back() << std::endl;
        
        std::cout << "Starting analysis..." << std::endl;
        Analyzer analyzer;
        auto results = analyzer.analyze(
            convertToVector(data.usd_data),
            convertToVector(data.ron_data),
            dates
        );
        
        std::cout << "Creating plots..." << std::endl;
        Plotter plotter;
        plotter.plot(results);
        
        std::cout << "Program completed successfully" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "\nERROR Details:" << std::endl;
        std::cerr << "- Message: " << e.what() << std::endl;
        std::cerr << "- Type: " << typeid(e).name() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "\nUnknown ERROR occurred!" << std::endl;
        return 1;
    }
}