#pragma once
#include <vector>
#include <string>
#include <iostream>

struct MFDFAResults {
    double hurst_exponent;
    double multifractal_spectrum_min;
    double multifractal_spectrum_max;
    std::vector<double> fluctuation_function;
    std::vector<double> generalized_hurst;
};

struct AnalysisResults {
    std::vector<std::string> dates;
    std::vector<double> usd_rates;
    std::vector<double> ron_rates;
    std::vector<double> integrated_lacunarity;
    std::vector<double> rolling_corr;
    MFDFAResults mfdfa_usd;
    MFDFAResults mfdfa_ron;
};

class Analyzer {
public:
    AnalysisResults analyze(const std::vector<double>& usd_data,
                          const std::vector<double>& ron_data,
                          const std::vector<std::string>& dates);

private:
    // Core analysis functions
    MFDFAResults performMFDFA(const std::vector<double>& data,
                            int order,
                            const std::vector<double>& q_values) const;
    
    // Rest of the declarations remain the same
    std::vector<double> calculateGBL(const std::vector<double>& data,
                                   const std::vector<int>& indices) const;
    std::vector<double> calculateGBL(const std::vector<double>& data) const;
    std::vector<double> calculateIntegratedLacunarity(const std::vector<double>& gbl_usd,
                                                     const std::vector<double>& gbl_ron) const;
    std::vector<double> calculateRollingCorrelation(const std::vector<double>& usd_data,
                                                   const std::vector<double>& ron_data);

    // Helper functions
    std::vector<double> polynomialDetrend(const std::vector<double>& data, int order) const;
    double calculateLocalFluctuation(const std::vector<double>& data) const;
    double calculateHurstExponent(double mfdfa_value) const;
    double calculateEfficiencyMeasure(double gbl_usd, double gbl_ron) const;
    double calculateMarketDepth(double gbl_usd, double gbl_ron) const;
    double computeBoxMass(const std::vector<double>& window) const;
    std::vector<double> calculateProfile(const std::vector<double>& data) const;
    std::vector<double> calculateVolatility(const std::vector<double>& data) const;
    std::vector<double> calculateVolatilityCluster(const std::vector<double>& volatility) const;
    std::vector<double> computeFluctuationFunction(const std::vector<double>& profile,
                                                  int scale, double q) const;
    double calculateWindowVolatility(const std::vector<double>& window) const;
    double calculateSingleVolatility(const std::vector<double>& window) const;

    static constexpr double sigma = 0.5; // Scaling parameter for volatility calculations
}; 