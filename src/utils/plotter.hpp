#pragma once
#include "../core/analyzer.hpp"
#include <string>
#include <fstream>

class Plotter {
public:
    Plotter();
    ~Plotter() = default;
    void plot(const AnalysisResults& results);

private:
    void saveDataToCSV(const AnalysisResults& results);
    void saveDataToJSON(const AnalysisResults& results);
    void generatePythonScript();
    void executePythonScript();
    
    const std::string DATA_FILE = "plot_data.csv";
    const std::string SCRIPT_FILE = "generate_plots.py";
}; 