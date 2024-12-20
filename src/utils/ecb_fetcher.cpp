#include "ecb_fetcher.hpp"
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <chrono>

using json = nlohmann::json;

const std::string ECBFetcher::BASE_URL = "https://sdw-wsrest.ecb.europa.eu/service/data/EXR/";

ECBFetcher::FetchResult ECBFetcher::fetchData(const std::string& start_date, const std::string& end_date) {
    FetchResult result;
    
    // Fetch EUR/USD rates
    result.usd_data = fetchHistoricalRates("USD", start_date, end_date);
    
    // Only proceed with RON if USD was successful
    if (!result.usd_data.empty()) {
        result.ron_data = fetchHistoricalRates("RON", start_date, end_date);
    }
    
    // Validate the result
    if (result.usd_data.empty() || result.ron_data.empty()) {
        throw std::runtime_error("Failed to fetch complete exchange rate data");
    }
    
    return result;
}

std::string ECBFetcher::buildQuery(const std::string& currency, 
                                   const std::string& start_date,
                                   const std::string& end_date) {
    // Format: D.{CURRENCY}.EUR.SP00.A
    return BASE_URL + "D." + currency + ".EUR.SP00.A"
           "?startPeriod=" + start_date + 
           "&endPeriod=" + end_date + 
           "&format=jsondata&detail=dataonly";
}

ECBFetcher::DataVector ECBFetcher::fetchHistoricalRates(const std::string& currency,
                                                       const std::string& start_date,
                                                       const std::string& end_date) {
    DataVector rates;
    std::string url = buildQuery(currency, start_date, end_date);
    
    std::cout << "Fetching data from: " << url << std::endl;

    try {
        cpr::Response r = cpr::Get(cpr::Url{url},
                                 cpr::Header{{"Accept", "application/json"}});
        
        if (r.status_code != 200) {
            throw std::runtime_error("Failed to fetch data: " + r.text);
        }

        std::cout << "Response status: " << r.status_code << std::endl;
        std::cout << "Response size: " << r.text.size() << " bytes" << std::endl;

        // Parse JSON response
        json response = json::parse(r.text);
        
        // Get the time periods array first
        const auto& timePeriods = response["structure"]["dimensions"]["observation"][0]["values"];
        
        std::cout << "Number of time periods: " << timePeriods.size() << std::endl;
        
        // Extract exchange rates from the correct JSON structure
        const auto& observations = response["dataSets"][0]["series"]["0:0:0:0:0"]["observations"];
        
        for (const auto& [index, values] : observations.items()) {
            if (values[0].is_null()) {
                std::cerr << "Skipping null value for index: " << index << std::endl;
                continue;
            }

            try {
                size_t idx = std::stoul(index);
                if (idx >= timePeriods.size()) {
                    std::cerr << "Invalid time period index: " << idx << std::endl;
                    continue;
                }

                DataPoint point;
                point.date = timePeriods[idx]["id"].get<std::string>();
                point.rate = values[0].get<double>();
                
                // Only include dates within our requested range
                if (point.date >= start_date && point.date <= end_date) {
                    rates.push_back(point);
                }
            } catch (const std::exception& e) {
                std::cerr << "Error parsing value for index " << index << ": " << e.what() << std::endl;
                continue;
            }
        }

        if (rates.empty()) {
            throw std::runtime_error("No valid data points found in the response");
        }

        // Sort by date
        std::sort(rates.begin(), rates.end(), 
                 [](const DataPoint& a, const DataPoint& b) {
                     return a.date < b.date;
                 });

        return rates;
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to fetch data: " + std::string(e.what()));
    }
}
