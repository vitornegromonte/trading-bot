cmake_minimum_required(VERSION 3.10)
project(CInTB LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(CURL REQUIRED)
find_package(nlohmann_json 3.2.0 REQUIRED)

include_directories(${CURL_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/src)

add_executable(CInTB
    src/main.cpp
    src/BinanceAPI.cpp
    src/YahooFinanceAPI.cpp
    src/SMAStrategy.cpp
    src/paperengine.cpp
    src/tradingbot.cpp
)

target_link_libraries(CInTB PRIVATE ${CURL_LIBRARIES} nlohmann_json::nlohmann_json)

# Yahoo Finance API demo executable
add_executable(yahoo_demo
    src/yahoo_demo.cpp
    src/YahooFinanceAPI.cpp
)

target_link_libraries(yahoo_demo PRIVATE ${CURL_LIBRARIES})

# Yahoo Finance backtest executable
add_executable(backtest_yahoo
    src/backtest_yahoo.cpp
    src/YahooFinanceAPI.cpp
    src/SMAStrategy.cpp
    src/paperengine.cpp
)

target_link_libraries(backtest_yahoo PRIVATE ${CURL_LIBRARIES} nlohmann_json::nlohmann_json)

# Multi-asset backtest executable
add_executable(multi_backtest
    src/multi_backtest.cpp
    src/YahooFinanceAPI.cpp
    src/SMAStrategy.cpp
    src/paperengine.cpp
)

target_link_libraries(multi_backtest PRIVATE ${CURL_LIBRARIES} nlohmann_json::nlohmann_json)

# Configurable backtest executable
add_executable(configurable_backtest
    src/configurable_backtest.cpp
    src/YahooFinanceAPI.cpp
    src/BinanceAPI.cpp
    src/SMAStrategy.cpp
    src/paperengine.cpp
)

target_link_libraries(configurable_backtest PRIVATE ${CURL_LIBRARIES} nlohmann_json::nlohmann_json)
