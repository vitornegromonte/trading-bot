#include "BinanceAPI.h"
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <sstream>
#include <iostream>

using json = nlohmann::json;

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

BinanceAPI::BinanceAPI() {
    curl_global_init(CURL_GLOBAL_DEFAULT);
}

BinanceAPI::~BinanceAPI() {
    curl_global_cleanup();
}

std::string build_klines_url(const std::string& symbol, const std::string& interval, int limit) {
    std::ostringstream ss;
    ss << "https://api.binance.com/api/v3/klines?symbol=" << symbol
       << "&interval=" << interval << "&limit=" << limit;
    return ss.str();
}

std::vector<Candle> BinanceAPI::fetch_klines(const std::string& symbol,
                                             const std::string& interval,
                                             int limit) {
    std::vector<Candle> result;
    CURL* curl = curl_easy_init();
    if(!curl) throw std::runtime_error("curl init failed");

    std::string readBuffer;
    std::string url = build_klines_url(symbol, interval, limit);

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "simple-cpp-bot/1.0");

    CURLcode res = curl_easy_perform(curl);
    if(res != CURLE_OK) {
        std::string err = curl_easy_strerror(res);
        curl_easy_cleanup(curl);
        throw std::runtime_error("curl perform failed: " + err);
    }
    curl_easy_cleanup(curl);

    try {
        auto j = json::parse(readBuffer);
        // j é um array de arrays; cada candle tem campos:
        // [0] openTime, [1] open, [2] high, [3] low, [4] close, [5] volume, ...
        for (auto &entry : j) {
            int64_t ts = entry[0].get<int64_t>();
            double open = std::stod(entry[1].get<std::string>());
            double high = std::stod(entry[2].get<std::string>());
            double low = std::stod(entry[3].get<std::string>());
            double close = std::stod(entry[4].get<std::string>());
            double vol = std::stod(entry[5].get<std::string>());
            result.emplace_back(ts, open, high, low, close, vol);
        }
    } catch (const std::exception& ex) {
        throw std::runtime_error(std::string("JSON parse error: ") + ex.what());
    }

    return result;
}
