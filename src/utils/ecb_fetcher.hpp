#pragma once
#include <string>
#include <vector>
#include <utility>
#include <cpr/cpr.h>

class ECBFetcher {
public:
    struct DataPoint {
        std::string date;
        double rate;
    };
    
    using DataVector = std::vector<DataPoint>;
    
    struct FetchResult {
        DataVector usd_data;
        DataVector ron_data;
    };

    static std::vector<DataPoint> fetchHistoricalRates(
        const std::string& currency,
        const std::string& start_date,
        const std::string& end_date
    );

    ECBFetcher() = default;
    FetchResult fetchData(const std::string& start_date, const std::string& end_date);

private:
    static const std::string BASE_URL;
    static std::string buildQuery(const std::string& currency, 
                                  const std::string& start_date,
                                  const std::string& end_date);
}; 