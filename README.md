# USD-EUR-RON Lacunarity Research Tool

A tool for analyzing the lacunarity (patterns of variability) in USD/EUR/RON exchange rate data using Python and C++.

## Overview

This tool combines the power of C++ for fast numerical computations with Python's matplotlib for visualization. It:

1. Fetches historical exchange rate data from the European Central Bank (ECB) API
2. Calculates lacunarity using sliding window analysis
3. Generates visualizations of the results
4. Exports findings to easily readable formats

## Prerequisites

- CMake 3.15 or higher
- C++ compiler with C++17 support
- Python 3.7 or higher with numpy
- Git

## Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/USD-EUR-RON-Lacunarity-research.git
   cd USD-EUR-RON-Lacunarity-research
   ```
2. Install Python dependencies:
   ```bash
   pip install matplotlib numpy
   ```
3. Build the project:
   ```bash
   # Clean any previous builds
   rm -rf build/
   # Configure and build
   cmake -B build -S .
   cmake --build build --config Release
   ```

## Usage

Run the analysis tool:
```bash
./build/Release/exchange_analyzer
```

When prompted:
1. Enter the start date (YYYY-MM-DD format)
2. Enter the end date (YYYY-MM-DD format)

The tool will automatically:
- Fetch EUR/USD and EUR/RON exchange rates from the ECB API
- Calculate lacunarity and correlation metrics
- Display interactive visualizations

## Output

The tool generates:
- Interactive plots showing exchange rate movements and lacunarity patterns
- Correlation analysis between EUR/USD and EUR/RON rates
- Hurst exponent calculations for trend analysis

## Technical Details

The project uses:
- matplotlib-cpp for visualization
- libcpr for ECB API requests
- nlohmann/json for API response parsing
- Modern C++17 features for efficient computation

## License

[Add your license information here]

## Contributing

[Add contribution guidelines here]