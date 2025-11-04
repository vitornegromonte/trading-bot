#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct BacktestConfig {
    bool enabled = true;
    std::string start_date = "2024-01-01";
    std::string end_date = "2024-11-01";
    double initial_capital = 10000.0;
};

struct StrategyConfig {
    std::string type = "sma";
    int short_period = 5;
    int long_period = 20;
    double trade_quantity = 1.0;
};

struct LiveTradingConfig {
    int poll_interval_seconds = 60;
    int fetch_limit = 200;
};

class Config {
public:
    std::string data_source = "yahoo";
    std::string symbol = "SPY";
    std::string interval = "1d";
    BacktestConfig backtest;
    StrategyConfig strategy;
    LiveTradingConfig live_trading;

    // Load from JSON file
    bool load_from_file(const std::string& filename) {
        try {
            std::ifstream file(filename);
            if (!file.is_open()) {
                return false;
            }
            
            json j;
            file >> j;
            
            if (j.contains("data_source")) data_source = j["data_source"];
            if (j.contains("symbol")) symbol = j["symbol"];
            if (j.contains("interval")) interval = j["interval"];
            
            if (j.contains("backtest")) {
                auto bt = j["backtest"];
                if (bt.contains("enabled")) backtest.enabled = bt["enabled"];
                if (bt.contains("start_date")) backtest.start_date = bt["start_date"];
                if (bt.contains("end_date")) backtest.end_date = bt["end_date"];
                if (bt.contains("initial_capital")) backtest.initial_capital = bt["initial_capital"];
            }
            
            if (j.contains("strategy")) {
                auto st = j["strategy"];
                if (st.contains("type")) strategy.type = st["type"];
                if (st.contains("short_period")) strategy.short_period = st["short_period"];
                if (st.contains("long_period")) strategy.long_period = st["long_period"];
                if (st.contains("trade_quantity")) strategy.trade_quantity = st["trade_quantity"];
            }
            
            if (j.contains("live_trading")) {
                auto lt = j["live_trading"];
                if (lt.contains("poll_interval_seconds")) live_trading.poll_interval_seconds = lt["poll_interval_seconds"];
                if (lt.contains("fetch_limit")) live_trading.fetch_limit = lt["fetch_limit"];
            }
            
            return true;
        } catch (const std::exception& e) {
            std::cerr << "Error loading config: " << e.what() << std::endl;
            return false;
        }
    }
    
    // Save to JSON file
    bool save_to_file(const std::string& filename) const {
        try {
            json j;
            j["data_source"] = data_source;
            j["symbol"] = symbol;
            j["interval"] = interval;
            
            j["backtest"]["enabled"] = backtest.enabled;
            j["backtest"]["start_date"] = backtest.start_date;
            j["backtest"]["end_date"] = backtest.end_date;
            j["backtest"]["initial_capital"] = backtest.initial_capital;
            
            j["strategy"]["type"] = strategy.type;
            j["strategy"]["short_period"] = strategy.short_period;
            j["strategy"]["long_period"] = strategy.long_period;
            j["strategy"]["trade_quantity"] = strategy.trade_quantity;
            
            j["live_trading"]["poll_interval_seconds"] = live_trading.poll_interval_seconds;
            j["live_trading"]["fetch_limit"] = live_trading.fetch_limit;
            
            std::ofstream file(filename);
            file << j.dump(2);
            return true;
        } catch (const std::exception& e) {
            std::cerr << "Error saving config: " << e.what() << std::endl;
            return false;
        }
    }
    
    void print() const {
        std::cout << "Configuration:\n";
        std::cout << "  Data Source: " << data_source << "\n";
        std::cout << "  Symbol: " << symbol << "\n";
        std::cout << "  Interval: " << interval << "\n";
        std::cout << "  Strategy: SMA(" << strategy.short_period << "," << strategy.long_period << ")\n";
        std::cout << "  Trade Qty: " << strategy.trade_quantity << "\n";
        if (backtest.enabled) {
            std::cout << "  Backtest: " << backtest.start_date << " to " << backtest.end_date << "\n";
            std::cout << "  Capital: $" << backtest.initial_capital << "\n";
        }
    }
};

#endif // CONFIG_H
