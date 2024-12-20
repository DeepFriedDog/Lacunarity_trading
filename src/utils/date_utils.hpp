#pragma once
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>

namespace DateUtils {
    // Convert string date to time_point
    std::chrono::system_clock::time_point parseDate(const std::string& date);
    
    // Add days to a date string
    std::string addDays(const std::string& date, int days);
    
    // Convert time_point to string date
    std::string formatDate(const std::chrono::system_clock::time_point& tp);
} 