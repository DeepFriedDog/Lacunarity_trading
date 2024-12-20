#include "analyzer.hpp"
#include <stdexcept>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <iostream>
#include <vector>
#include <limits>

/**
 * @brief Performs a multifractal detrended fluctuation analysis (MFDFA) on the given data series.
 * 
 * This function is a placeholder, intended to align with the methodology described in
 * "Exploring the Economic Feedback Mechanisms Between EUR/USD and EUR/RON Using Lacunarity-Augmented
 * Multifractal Detrended Fluctuation Analysis." It should:
 * 1. Detrend the series using polynomial fitting (e.g., linear or higher-order),
 * 2. Calculate the fluctuation function Fq(s) across multiple box sizes s and moments q,
 * 3. Compute the generalized Hurst exponents h(q) and multifractal spectrum.
 *
 * @param data Input time series for which MFDFA should be calculated (e.g., log returns).
 * @param order Polynomial order for detrending each window segment.
 * @param q_values A list of q moments used for the multifractal analysis.
 * @return A user-defined structure or placeholder object with MFDFA metrics 
 *         (generalized Hurst exponents, multifractal spectrum, etc.).
 */
MFDFAResults Analyzer::performMFDFA(
    const std::vector<double>& data,
    int order,
    const std::vector<double>& q_values) const
{
    // Placeholder for actual MFDFA logic. While the steps below are
    // purely illustrative, they align with the standard MFDFA methodology.

    MFDFAResults results;
    if (data.empty()) {
        std::cerr << "[Warning] Empty data received in performMFDFA." << std::endl;
        return results;
    }

    // Example: indicate that MFDFA is in progress
    std::cout << "[MFDFA] Performing multifractal analysis (order=" << order << ")..." << std::endl;

    // For real usage:
    // 1) Compute the cumulative sum of (data - mean(data)),
    // 2) Subdivide into windows, detrend each window with a polynomial fit,
    // 3) Compute fluctuation function for each scale s and moment q,
    // 4) Derive the generalized Hurst exponent h(q) and the multifractal spectrum.

    // Here, we simply show a placeholder assignment indicating no real calculations yet:
    results.hurst_exponent = 0.5;        // Example placeholder
    results.multifractal_spectrum_min = 0.4;
    results.multifractal_spectrum_max = 0.7;
    // ... further placeholders as needed

    return results;
}

/**
 * @brief Primary analysis function, which integrates MFDFA, GBL, correlation, and other metrics.
 * 
 * This method exemplifies how to orchestrate multiple analysis routines to align with the
 * multifractal-lacunarity framework described in the research paper.
 *
 * @param usd_data Vector of EUR/USD exchange rate data (log returns or prices, based on usage).
 * @param ron_data Vector of EUR/RON exchange rate data (log returns or prices, based on usage).
 * @param dates Corresponding dates for each data point (optional; used for labeling results).
 * @return Populated AnalysisResults structure containing all relevant computed metrics.
 */
AnalysisResults Analyzer::analyze(
    const std::vector<double>& usd_data,
    const std::vector<double>& ron_data,
    const std::vector<std::string>& dates)
{
    AnalysisResults results;
    try
    {
        std::cout << "[Analyzer] Starting analysis..." << std::endl;

        // Calculate GBL for both currency pairs
        std::cout << "[Analyzer] Calculating GBL values..." << std::endl;
        auto gbl_usd = calculateGBL(usd_data);
        auto gbl_ron = calculateGBL(ron_data);
        
        // Calculate integrated lacunarity
        std::cout << "[Analyzer] Computing integrated lacunarity..." << std::endl;
        results.integrated_lacunarity = calculateIntegratedLacunarity(gbl_usd, gbl_ron);
        
        // Store the original rates
        results.usd_rates = usd_data;
        results.ron_rates = ron_data;

        // MFDFA calculations
        const int detrend_order = 1;
        std::vector<double> q_values = { -5.0, -2.0, -1.0, 0.0, 1.0, 2.0, 5.0 };
        results.mfdfa_usd = performMFDFA(usd_data, detrend_order, q_values);
        results.mfdfa_ron = performMFDFA(ron_data, detrend_order, q_values);

        // Calculate rolling correlation
        std::cout << "[Analyzer] Calculating rolling correlation..." << std::endl;
        results.rolling_corr = calculateRollingCorrelation(usd_data, ron_data);

        // Attach date labels
        results.dates = dates;

        // Debug output
        std::cout << "[Analyzer] Analysis summary:" << std::endl;
        std::cout << "- GBL USD size: " << gbl_usd.size() << std::endl;
        std::cout << "- GBL RON size: " << gbl_ron.size() << std::endl;
        std::cout << "- Integrated lacunarity size: " << results.integrated_lacunarity.size() << std::endl;
        std::cout << "- Rolling correlation size: " << results.rolling_corr.size() << std::endl;
        std::cout << "- Number of dates: " << results.dates.size() << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << "[Analyzer] Error during analysis: " << e.what() << std::endl;
        throw;
    }

    return results;
}

/**
 * @brief Compute integrated lacunarity values from GBL outputs of two currency pairs.
 *
 * This function combines or “averages” the GBL-based lacunarity values from two data sets,
 * for instance EUR/USD and EUR/RON, to form an integrated measure that can highlight
 * simultaneous market inefficiencies across both currencies.
 *
 * @param gbl_usd Lacunarity values (GBL) for the EUR/USD exchange data.
 * @param gbl_ron Lacunarity values (GBL) for the EUR/RON exchange data.
 * @return Vector of integrated lacunarity values.
 */
std::vector<double> Analyzer::calculateIntegratedLacunarity(
    const std::vector<double>& gbl_usd,
    const std::vector<double>& gbl_ron
) const
{
    std::cout << "\n[Analyzer] Debug - Integrated Lacunarity Calculation:\n";
    std::cout << "Input sizes - GBL USD: " << gbl_usd.size() << ", GBL RON: " << gbl_ron.size() << std::endl;

    size_t size = std::min(gbl_usd.size(), gbl_ron.size());
    std::vector<double> integrated_lacunarity;
    integrated_lacunarity.reserve(size);

    for (size_t i = 0; i < size; ++i) {
        // Example: combine by averaging
        double val = 0.5 * (gbl_usd[i] + gbl_ron[i]);
        integrated_lacunarity.push_back(val);
    }

    std::cout << "[Analyzer] Completed integrated lacunarity calculation. Output size: "
              << integrated_lacunarity.size() << std::endl;
    return integrated_lacunarity;
}

/**
 * @brief Calculate rolling correlation between two data series (e.g., EUR/USD and EUR/RON).
 *
 * The calculation uses a fixed window size to compute correlation, aligning
 * with standard rolling correlation techniques. This helps detect time-varying
 * correlation patterns and potential feedback loops.
 *
 * @param usd_data Data for the first currency pair (e.g., EUR/USD).
 * @param ron_data Data for the second currency pair (e.g., EUR/RON).
 * @return Vector of rolling correlation values.
 */
std::vector<double> Analyzer::calculateRollingCorrelation(
    const std::vector<double>& usd_data,
    const std::vector<double>& ron_data)
{
    const size_t window_size = 30; // As per paper's methodology
    std::vector<double> rolling_corr;
    rolling_corr.reserve(usd_data.size() - window_size + 1);

    for (size_t i = 0; i <= usd_data.size() - window_size; ++i) {
        // Extract window for both series
        std::vector<double> usd_window(usd_data.begin() + i, usd_data.begin() + i + window_size);
        std::vector<double> ron_window(ron_data.begin() + i, ron_data.begin() + i + window_size);

        // Calculate means
        double mean_usd = std::accumulate(usd_window.begin(), usd_window.end(), 0.0) / window_size;
        double mean_ron = std::accumulate(ron_window.begin(), ron_window.end(), 0.0) / window_size;

        // Calculate covariance and variances
        double covariance = 0.0, var_usd = 0.0, var_ron = 0.0;
        for (size_t j = 0; j < window_size; ++j) {
            double usd_diff = usd_window[j] - mean_usd;
            double ron_diff = ron_window[j] - mean_ron;
            covariance += usd_diff * ron_diff;
            var_usd += usd_diff * usd_diff;
            var_ron += ron_diff * ron_diff;
        }

        // Normalize by window size
        covariance /= window_size;
        var_usd /= window_size;
        var_ron /= window_size;

        // Calculate correlation coefficient
        double correlation = 0.0;
        if (var_usd > 0 && var_ron > 0) {
            correlation = covariance / std::sqrt(var_usd * var_ron);
        }

        rolling_corr.push_back(correlation);
    }

    return rolling_corr;
}

/**
 * @brief Compute Gliding Box Lacunarity (GBL) for the given data and a set of indices.
 *
 * This version of calculateGBL aligns with the “two-argument” signature used in the codebase,
 * allowing selection of specific starting points (indices) for analysis (e.g., detecting
 * local structural breaks or gaps).
 *
 * @param data A vector of data points (e.g., log returns) for which GBL is computed.
 * @param indices A list of start indices where each gliding box should be positioned.
 * @return A vector of lacunarity values, one for each index.
 */
std::vector<double> Analyzer::calculateGBL(
    const std::vector<double>& data,
    const std::vector<int>& indices) const
{
    if (data.empty() || indices.empty()) {
        throw std::runtime_error("Empty input data or indices in calculateGBL");
    }

    const size_t window_size = 50;  
    std::vector<double> gbl_values;
    gbl_values.reserve(indices.size());

    for (int idx : indices) {
        if (idx < 0 || static_cast<size_t>(idx + window_size) > data.size()) {
            continue;
        }

        // Extract window
        std::vector<double> window(data.begin() + idx, data.begin() + idx + window_size);

        double mean = std::accumulate(window.begin(), window.end(), 0.0) / window_size;

        // Calculate box mass (density)
        double box_mass = 0.0;
        for (const auto& val : window) {
            box_mass += std::abs(val - mean);
        }
        box_mass /= window_size;

        // Calculate variance for lacunarity
        double variance = 0.0;
        for (const auto& val : window) {
            double diff = std::abs(val - mean) - box_mass;
            variance += diff * diff;
        }
        variance /= window_size;

        // Calculate lacunarity value
        double lac = (box_mass != 0.0) ? variance / (box_mass * box_mass) : 0.0;
        gbl_values.push_back(lac);
    }

    return gbl_values;
}

/**
 * @brief Compute Gliding Box Lacunarity (GBL) for the entire data series,
 * using a sequential sliding window across the entire range.
 *
 * This version processes all possible windows in the time series to generate a full
 * lacunarity profile, helping detect structural breaks, liquidity gaps, or localized
 * market inefficiencies.
 *
 * @param data A vector of data points (e.g., log returns) for which GBL is computed.
 * @return A vector of lacunarity values across all valid windows.
 */
std::vector<double> Analyzer::calculateGBL(const std::vector<double>& data) const
{
    if (data.empty()) {
        throw std::runtime_error("Empty input data in calculateGBL");
    }

    const size_t window_size = 50;  
    std::vector<double> gbl_values;
    if (data.size() < window_size) {
        return gbl_values;
    }
    gbl_values.reserve(data.size() - window_size + 1);

    for (size_t i = 0; i + window_size <= data.size(); ++i) {
        std::vector<double> window(data.begin() + i, data.begin() + i + window_size);

        double mean = std::accumulate(window.begin(), window.end(), 0.0) / window_size;

        // Calculate box mass (density)
        double box_mass = 0.0;
        for (const auto& val : window) {
            box_mass += std::abs(val - mean);
        }
        box_mass /= window_size;

        // Calculate variance for lacunarity
        double variance = 0.0;
        for (const auto& val : window) {
            double diff = std::abs(val - mean) - box_mass;
            variance += diff * diff;
        }
        variance /= window_size;

        // Calculate lacunarity value
        double lac = (box_mass != 0.0) ? variance / (box_mass * box_mass) : 0.0;
        gbl_values.push_back(lac);
    }

    return gbl_values;
}

