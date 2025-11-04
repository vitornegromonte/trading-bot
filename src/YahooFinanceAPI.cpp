#include "YahooFinanceAPI.h"
#include <curl/curl.h>
#include <stdexcept>
#include <sstream>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <vector>

// For JSON parsing (reuse nlohmann/json)
#include <nlohmann/json.hpp>
using json = nlohmann::json;

static size_t YFWriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

static size_t YFFileWriteCallback(void* contents, size_t size, size_t nmemb, FILE* stream) {
    return fwrite(contents, size, nmemb, stream);
}

YahooFinanceAPI::YahooFinanceAPI() {
    curl_global_init(CURL_GLOBAL_DEFAULT);
    // Use the chart API which is more reliable and doesn't require authentication
    _base_url = "https://query1.finance.yahoo.com/v8/finance/chart/{ticker}?period1={start_time}&period2={end_time}&interval={interval}";
    _interval = DAILY;
}

YahooFinanceAPI::~YahooFinanceAPI() {
    curl_global_cleanup();
}

void YahooFinanceAPI::set_interval(Interval interval) {
    _interval = interval;
}

// Converte string de data YYYY-MM-DD para timestamp Unix
std::string YahooFinanceAPI::timestamp_from_string(const std::string& date) {
    struct std::tm time = {0,0,0,0,0,0,0,0,0};
    std::istringstream ss(date);
    ss >> std::get_time(&time, "%Y-%m-%d");
    if(ss.fail()) {
        std::cerr << "ERROR: Cannot parse date string (" << date << "); required format YYYY-MM-DD" << std::endl;
        throw std::runtime_error("Invalid date format");
    }
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;
    std::time_t epoch = std::mktime(&time);
    return std::to_string(epoch);
}

bool YahooFinanceAPI::string_replace(std::string& str, const std::string& from, const std::string& to) {
    size_t start = str.find(from);
    if(start == std::string::npos) {
        return false;
    }
    str.replace(start, from.length(), to);
    return true;
}

std::string YahooFinanceAPI::build_url(const std::string& ticker, 
                                      const std::string& start_date,
                                      const std::string& end_date) {
    std::string url = _base_url;
    string_replace(url, "{ticker}", ticker);
    string_replace(url, "{start_time}", timestamp_from_string(start_date));
    string_replace(url, "{end_time}", timestamp_from_string(end_date));
    string_replace(url, "{interval}", get_api_interval_value(_interval));
    return url;
}

void YahooFinanceAPI::download_file(const std::string& url, const std::string& filename) {
    CURL* curl = curl_easy_init();
    if(!curl) throw std::runtime_error("curl init failed");

    std::string readBuffer;
    
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, YFWriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0");

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    if(res != CURLE_OK) {
        throw std::runtime_error("curl perform failed: " + std::string(curl_easy_strerror(res)));
    }
    
    // Save to file
    std::ofstream outfile(filename);
    if(!outfile.is_open()) {
        throw std::runtime_error("Cannot open file for writing: " + filename);
    }
    outfile << readBuffer;
    outfile.close();
}

// Parse JSON response (from chart API) to vector of Candles
std::vector<Candle> YahooFinanceAPI::parse_csv_to_candles(const std::string& filename) {
    std::vector<Candle> result;
    std::ifstream file(filename);
    if(!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    
    std::string content = buffer.str();
    
    try {
        auto j = json::parse(content);
        
        // Navigate to chart.result[0]
        if (!j.contains("chart") || !j["chart"].contains("result") || 
            j["chart"]["result"].is_null() || j["chart"]["result"].empty()) {
            throw std::runtime_error("Unexpected JSON: missing chart/result");
        }

        auto result_node = j["chart"]["result"][0];
        auto timestamps = result_node["timestamp"];
        auto indicators = result_node["indicators"];
        
        if (timestamps.is_null() || timestamps.empty()) {
            return result; // No data available for this range
        }
        
        if (indicators.is_null() || !indicators.contains("quote") || indicators["quote"].empty()) {
            return result; // No quote data
        }

        auto quote = indicators["quote"][0];
        auto opens = quote["open"];
        auto highs = quote["high"];
        auto lows = quote["low"];
        auto closes = quote["close"];
        auto volumes = quote["volume"];

        // timestamps are seconds since epoch
        size_t n = timestamps.size();
        for (size_t i = 0; i < n; ++i) {
            if (closes[i].is_null()) continue; // skip missing data
            
            int64_t ts = timestamps[i].get<int64_t>() * 1000; // to ms
            double open = opens[i].is_null() ? 0.0 : opens[i].get<double>();
            double high = highs[i].is_null() ? open : highs[i].get<double>();
            double low = lows[i].is_null() ? open : lows[i].get<double>();
            double close = closes[i].get<double>();
            double vol = volumes[i].is_null() ? 0.0 : volumes[i].get<double>();
            
            result.emplace_back(ts, open, high, low, close, vol);
        }
    } catch (const json::exception& e) {
        throw std::runtime_error(std::string("JSON parse error: ") + e.what());
    }
    
    return result;
}

std::vector<Candle> YahooFinanceAPI::get_ticker_data(const std::string& ticker, 
                                                    const std::string& start_date, 
                                                    const std::string& end_date) {
    std::string url = build_url(ticker, start_date, end_date);
    std::time_t now = std::time(0);
    std::string tmp_filename = "tmp_" + ticker + "_" + std::to_string(now) + ".csv";
    
    download_file(url, tmp_filename);
    std::vector<Candle> candles = parse_csv_to_candles(tmp_filename);
    
    // Clean up temporary file
    std::remove(tmp_filename.c_str());
    
    return candles;
}

std::string YahooFinanceAPI::download_ticker_csv(const std::string& ticker, 
                                                const std::string& start_date, 
                                                const std::string& end_date) {
    std::string url = build_url(ticker, start_date, end_date);
    std::time_t now = std::time(0);
    std::string filename = ticker + "_" + std::to_string(now) + ".json";
    
    download_file(url, filename);
    
    return filename;
}

// Implementation of the ExchangeAPI interface using limit -> date range conversion
std::vector<Candle> YahooFinanceAPI::fetch_klines(const std::string& symbol,
                                                  const std::string& interval,
                                                  int limit) {
    // Map interval string to Interval enum
    if(interval == "1d") _interval = DAILY;
    else if(interval == "1wk") _interval = WEEKLY;
    else if(interval == "1mo") _interval = MONTHLY;
    else if(interval == "1m") _interval = DAILY; // 1 minute -> use daily as fallback
    else _interval = DAILY;
    
    // Calculate approximate date range based on limit and interval
    // Use yesterday as end date to avoid requesting future data
    std::time_t now = std::time(0) - (24 * 60 * 60); // yesterday
    std::time_t start_time;
    
    if(_interval == DAILY) {
        start_time = now - (limit * 24 * 60 * 60); // limit days
    } else if(_interval == WEEKLY) {
        start_time = now - (limit * 7 * 24 * 60 * 60); // limit weeks
    } else { // MONTHLY
        start_time = now - (limit * 30 * 24 * 60 * 60); // limit * ~30 days
    }
    
    // Convert timestamps to date strings
    struct std::tm* start_tm = std::localtime(&start_time);
    struct std::tm* end_tm = std::localtime(&now);
    
    char start_buf[11], end_buf[11];
    std::strftime(start_buf, sizeof(start_buf), "%Y-%m-%d", start_tm);
    std::strftime(end_buf, sizeof(end_buf), "%Y-%m-%d", end_tm);
    
    std::string start_date(start_buf);
    std::string end_date(end_buf);
    
    std::vector<Candle> candles = get_ticker_data(symbol, start_date, end_date);
    
    // Limit to requested number of candles (most recent)
    if((int)candles.size() > limit) {
        candles = std::vector<Candle>(candles.end() - limit, candles.end());
    }
    
    return candles;
}
