cmake_minimum_required(VERSION 3.10)
project(USD-EUR-RON-Lacunarity-research)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Python for matplotlib-cpp
find_package(Python3 COMPONENTS Development NumPy REQUIRED)
find_package(Python3 COMPONENTS Interpreter REQUIRED)

# Add CPR for HTTP requests
include(FetchContent)
FetchContent_Declare(
    cpr
    GIT_REPOSITORY https://github.com/libcpr/cpr.git
    GIT_TAG 1.10.x
)
set(CPR_BUILD_TESTS OFF)
set(CPR_ENABLE_SSL ON)
FetchContent_MakeAvailable(cpr)

# Add nlohmann/json for JSON parsing
FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz
)
FetchContent_MakeAvailable(json)

# Add matplotlib-cpp
add_library(matplotlib_cpp INTERFACE)
target_include_directories(matplotlib_cpp 
    INTERFACE 
    ${Python3_INCLUDE_DIRS}
    ${Python3_NumPy_INCLUDE_DIRS}
    external/matplotlib-cpp
)
target_link_libraries(matplotlib_cpp
    INTERFACE
    Python3::Python
    Python3::NumPy
)

# Create executable
add_executable(exchange_analyzer
    src/main.cpp
    src/core/analyzer.cpp
    src/utils/data_loader.cpp
    src/utils/ecb_fetcher.cpp
    src/utils/plotter.cpp
)

# Link libraries
target_link_libraries(exchange_analyzer
    PRIVATE
    cpr::cpr
    nlohmann_json::nlohmann_json
    matplotlib_cpp
)

add_custom_command(TARGET exchange_analyzer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:cpr>
        $<TARGET_FILE:libcurl>
        $<TARGET_FILE:zlib>
        $<TARGET_FILE_DIR:exchange_analyzer>
) 

# Add source directory to include paths
target_include_directories(exchange_analyzer PRIVATE 
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/external/matplotlib-cpp
) 