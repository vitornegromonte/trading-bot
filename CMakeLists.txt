cmake_minimum_required(VERSION 3.10)
project(simple_trading_bot LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(CURL REQUIRED)
find_package(nlohmann_json 3.2.0 REQUIRED)

include_directories(${CURL_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/src)

add_executable(simple_trading_bot
    src/main.cpp
    src/BinanceAPI.cpp
    src/SMAStrategy.cpp
    src/paperengine.cpp
    src/tradingbot.cpp
)

target_link_libraries(simple_trading_bot PRIVATE ${CURL_LIBRARIES} nlohmann_json::nlohmann_json)
