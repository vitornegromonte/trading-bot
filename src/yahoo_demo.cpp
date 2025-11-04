#include <iostream>
#include "YahooFinanceAPI.h"

int main() {
    std::cout << "Yahoo Finance API Demo" << std::endl;
    
    YahooFinanceAPI api;
    
    // Get daily data for SPY (S&P 500 ETF)
    std::cout << "\n1. Fetching daily SPY data (2024-10-01 to 2024-11-01)..." << std::endl;
    api.set_interval(DAILY);
    try {
        auto candles = api.get_ticker_data("SPY", "2024-10-01", "2024-11-01");
        std::cout << "   Retrieved " << candles.size() << " candles" << std::endl;
        if (!candles.empty()) {
            auto& last = candles.back();
            std::cout << "   Last candle: O=" << last.open << " H=" << last.high 
                     << " L=" << last.low << " C=" << last.close << " V=" << last.volume << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "   Error: " << e.what() << std::endl;
    }

    // Get weekly data for BTC-USD
    std::cout << "\n2. Fetching weekly BTC-USD data (2024-01-01 to 2024-11-01)..." << std::endl;
    api.set_interval(WEEKLY);
    try {
        auto candles = api.get_ticker_data("BTC-USD", "2024-01-01", "2024-11-01");
        std::cout << "   Retrieved " << candles.size() << " candles" << std::endl;
        if (!candles.empty()) {
            auto& first = candles.front();
            auto& last = candles.back();
            std::cout << "   First: C=" << first.close << ", Last: C=" << last.close << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "   Error: " << e.what() << std::endl;
    }
    
    // Download CSV file
    std::cout << "\n3. Downloading AAPL monthly data to CSV..." << std::endl;
    api.set_interval(MONTHLY);
    try {
        std::string filename = api.download_ticker_csv("AAPL", "2020-01-01", "2024-11-01");
        std::cout << "   File saved: " << filename << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "   Error: " << e.what() << std::endl;
    }
    
    // Use fetch_klines interface (ExchangeAPI compatible)
    std::cout << "\n4. Using fetch_klines interface (last 30 days of TSLA)..." << std::endl;
    try {
        auto candles = api.fetch_klines("TSLA", "1d", 30);
        std::cout << "   Retrieved " << candles.size() << " candles" << std::endl;
        if (!candles.empty()) {
            auto& last = candles.back();
            std::cout << "   Latest close: $" << last.close << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "   Error: " << e.what() << std::endl;
    }
    
    std::cout << "\n=== Demo Complete ===" << std::endl;
    return 0;
}
