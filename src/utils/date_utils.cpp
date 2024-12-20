#include "date_utils.hpp"

namespace DateUtils {
    std::chrono::system_clock::time_point parseDate(const std::string& date) {
        std::tm tm = {};
        std::istringstream ss(date);
        ss >> std::get_time(&tm, "%Y-%m-%d");
        return std::chrono::system_clock::from_time_t(std::mktime(&tm));
    }

    std::string addDays(const std::string& date, int days) {
        auto tp = parseDate(date);
        tp += std::chrono::hours(24 * days);
        return formatDate(tp);
    }

    std::string formatDate(const std::chrono::system_clock::time_point& tp) {
        auto time = std::chrono::system_clock::to_time_t(tp);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time), "%Y-%m-%d");
        return ss.str();
    }
} 