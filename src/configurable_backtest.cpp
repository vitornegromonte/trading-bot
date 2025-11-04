#include <iostream>
#include <iomanip>
#include <cstring>
#include "config.h"
#include "YahooFinanceAPI.h"
#include "BinanceAPI.h"
#include "SMAStrategy.h"
#include "paperengine.h"

void print_usage(const char* prog) {
    std::cout << "Usage: " << prog << " [options]\n\n";
    std::cout << "Options:\n";
    std::cout << "  --config FILE          Load configuration from FILE (default: config.json)\n";
    std::cout << "  --symbol SYMBOL        Override symbol (e.g., SPY, AAPL, BTC-USD)\n";
    std::cout << "  --source SOURCE        Data source: yahoo or binance (default: yahoo)\n";
    std::cout << "  --start DATE           Backtest start date YYYY-MM-DD\n";
    std::cout << "  --end DATE             Backtest end date YYYY-MM-DD\n";
    std::cout << "  --capital AMOUNT       Initial capital (default: 10000)\n";
    std::cout << "  --sma-short N          Short SMA period (default: 5)\n";
    std::cout << "  --sma-long N           Long SMA period (default: 20)\n";
    std::cout << "  --qty AMOUNT           Trade quantity (default: 1.0)\n";
    std::cout << "  --interval INTERVAL    Data interval: 1d, 1wk, 1mo (default: 1d)\n";
    std::cout << "  --help                 Show this help message\n";
    std::cout << "\nExamples:\n";
    std::cout << "  " << prog << " --symbol TSLA --sma-short 10 --sma-long 50\n";
    std::cout << "  " << prog << " --symbol BTC-USD --start 2023-01-01\n";
    std::cout << "  " << prog << " --config my_config.json\n";
}

void print_separator(char c = '=') {
    std::cout << std::string(80, c) << std::endl;
}

int main(int argc, char** argv) {
    Config config;
    std::string config_file = "config.json";
    
    // Parse command line arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        
        if (arg == "--help" || arg == "-h") {
            print_usage(argv[0]);
            return 0;
        } else if (arg == "--config" && i + 1 < argc) {
            config_file = argv[++i];
        } else if (arg == "--symbol" && i + 1 < argc) {
            config.symbol = argv[++i];
        } else if (arg == "--source" && i + 1 < argc) {
            config.data_source = argv[++i];
        } else if (arg == "--start" && i + 1 < argc) {
            config.backtest.start_date = argv[++i];
        } else if (arg == "--end" && i + 1 < argc) {
            config.backtest.end_date = argv[++i];
        } else if (arg == "--capital" && i + 1 < argc) {
            config.backtest.initial_capital = std::stod(argv[++i]);
        } else if (arg == "--sma-short" && i + 1 < argc) {
            config.strategy.short_period = std::stoi(argv[++i]);
        } else if (arg == "--sma-long" && i + 1 < argc) {
            config.strategy.long_period = std::stoi(argv[++i]);
        } else if (arg == "--qty" && i + 1 < argc) {
            config.strategy.trade_quantity = std::stod(argv[++i]);
        } else if (arg == "--interval" && i + 1 < argc) {
            config.interval = argv[++i];
        }
    }
    
    // Try to load config file (command line args override file settings)
    Config file_config;
    if (file_config.load_from_file(config_file)) {
        // Merge: use file config as base, override with command line
        if (argc <= 2) { // No overrides, use file config entirely
            config = file_config;
        } else {
            // Keep command line overrides, use file for non-specified values
            if (std::string(argv[argc-1]).find("--symbol") == std::string::npos) {
                if (config.symbol == "SPY") config.symbol = file_config.symbol;
            }
            // Add more merge logic as needed
        }
    }
    
    print_separator();
    std::cout << "=== CONFIGURABLE BACKTEST ===" << std::endl;
    print_separator();
    std::cout << "\n";
    config.print();
    std::cout << "\n";
    
    // Fetch data based on config
    std::vector<Candle> candles;
    
    if (config.data_source == "yahoo") {
        YahooFinanceAPI api;
        
        // Set interval
        if (config.interval == "1d") api.set_interval(DAILY);
        else if (config.interval == "1wk") api.set_interval(WEEKLY);
        else if (config.interval == "1mo") api.set_interval(MONTHLY);
        
        try {
            candles = api.get_ticker_data(config.symbol, 
                                         config.backtest.start_date, 
                                         config.backtest.end_date);
        } catch (const std::exception& e) {
            std::cerr << "Error fetching data: " << e.what() << std::endl;
            return 1;
        }
    } else if (config.data_source == "binance") {
        // Note: Binance uses limit-based fetching, not date ranges
        BinanceAPI api;
        try {
            candles = api.fetch_klines(config.symbol, config.interval, config.live_trading.fetch_limit);
        } catch (const std::exception& e) {
            std::cerr << "Error fetching data: " << e.what() << std::endl;
            return 1;
        }
    } else {
        std::cerr << "Unknown data source: " << config.data_source << std::endl;
        return 1;
    }
    
    if (candles.empty()) {
        std::cerr << "No data retrieved!\n";
        return 1;
    }
    
    std::cout << "Retrieved " << candles.size() << " candles\n";
    std::cout << "Price range: $" << std::fixed << std::setprecision(2) 
              << candles.front().close << " -> $" << candles.back().close << "\n\n";
    
    // Initialize strategy and engine
    SMAStrategy strategy(config.strategy.short_period, 
                        config.strategy.long_period, 
                        config.strategy.trade_quantity);
    PaperEngine engine(config.backtest.initial_capital);
    
    print_separator();
    std::cout << "BACKTEST SIMULATION\n";
    print_separator();
    
    int trade_count = 0;
    int min_window = std::max(config.strategy.long_period, 20);
    
    // Run simulation
    for (size_t i = min_window; i < candles.size(); ++i) {
        std::vector<Candle> window(candles.begin(), candles.begin() + i + 1);
        Order signal = strategy.on_candles(window, config.symbol);
        
        if (signal.status != "NONE") {
            std::cout << "\n[Candle " << std::setw(3) << i << "] " 
                     << signal.side << " signal @ $" 
                     << std::fixed << std::setprecision(2) << signal.price << "\n";
            
            Order filled = engine.execute(signal);
            
            if (filled.status == "FILLED") {
                trade_count++;
                std::cout << "            Executed: " << filled.side 
                         << " " << filled.qty << " @ $" << filled.price << "\n";
                
                double nav = engine.get_cash() + engine.get_position_qty() * signal.price;
                std::cout << "            Portfolio: Cash=$" << engine.get_cash()
                         << " | Pos=" << engine.get_position_qty()
                         << " | NAV=$" << nav << "\n";
            } else {
                std::cout << "            " << filled.status << "\n";
            }
        }
    }
    
    print_separator();
    std::cout << "RESULTS\n";
    print_separator();
    
    // Calculate final metrics
    double final_price = candles.back().close;
    double final_cash = engine.get_cash();
    double final_position = engine.get_position_qty();
    double final_nav = final_cash + final_position * final_price;
    double profit = final_nav - config.backtest.initial_capital;
    double profit_pct = (profit / config.backtest.initial_capital) * 100.0;
    
    std::cout << "\nTrades Executed: " << trade_count << "\n";
    std::cout << "Final Cash: $" << std::fixed << std::setprecision(2) << final_cash << "\n";
    std::cout << "Final Position: " << final_position << " @ $" << final_price << "\n";
    std::cout << "Final NAV: $" << final_nav << "\n";
    std::cout << "Profit/Loss: $" << profit << " (" 
             << std::showpos << profit_pct << std::noshowpos << "%)\n";
    
    // Buy & hold comparison
    double bh_shares = config.backtest.initial_capital / candles[min_window].close;
    double bh_nav = bh_shares * final_price;
    double bh_profit = bh_nav - config.backtest.initial_capital;
    double bh_pct = (bh_profit / config.backtest.initial_capital) * 100.0;
    
    std::cout << "\nBuy & Hold Comparison:\n";
    std::cout << "  NAV: $" << bh_nav << "\n";
    std::cout << "  P/L: $" << bh_profit << " (" 
             << std::showpos << bh_pct << std::noshowpos << "%)\n";
    
    double diff = profit - bh_profit;
    if (diff > 0) {
        std::cout << "\n🎉 Strategy OUTPERFORMED by $" << diff << "\n";
    } else {
        std::cout << "\n📊 Strategy underperformed by $" << -diff << "\n";
    }
    
    print_separator();
    
    return 0;
}
