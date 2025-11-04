#include <iostream>
#include <iomanip>
#include "YahooFinanceAPI.h"
#include "SMAStrategy.h"
#include "paperengine.h"

void print_separator() {
    std::cout << std::string(80, '=') << std::endl;
}

int main() {
    print_separator();
    std::cout << "YAHOO FINANCE BACKTEST - SMA STRATEGY" << std::endl;
    print_separator();
    
    // Setup
    YahooFinanceAPI api;
    api.set_interval(DAILY);
    
    // Test with SPY (S&P 500 ETF) - high liquidity, good for backtesting
    std::string symbol = "SPY";
    std::string start_date = "2024-01-01";
    std::string end_date = "2024-11-01";
    
    std::cout << "\nFetching " << symbol << " data from " << start_date << " to " << end_date << "...\n";
    
    std::vector<Candle> all_candles;
    try {
        all_candles = api.get_ticker_data(symbol, start_date, end_date);
    } catch (const std::exception& e) {
        std::cerr << "Error fetching data: " << e.what() << std::endl;
        return 1;
    }
    
    if (all_candles.empty()) {
        std::cerr << "No data retrieved!" << std::endl;
        return 1;
    }
    
    std::cout << "Retrieved " << all_candles.size() << " daily candles\n";
    std::cout << "Price range: $" << all_candles.front().close << " -> $" << all_candles.back().close << "\n\n";
    
    // Initialize strategy and paper engine
    // Use smaller periods for more trades: 5-day vs 20-day SMA
    SMAStrategy strategy(5, 20, 1.0);  // Trade 1 share at a time
    PaperEngine engine(10000.0);  // Start with $10,000
    
    std::cout << "Strategy: SMA(5, 20) Crossover\n";
    std::cout << "Initial Capital: $" << std::fixed << std::setprecision(2) << engine.get_cash() << "\n\n";
    
    print_separator();
    std::cout << "SIMULATION START\n";
    print_separator();
    
    int trade_count = 0;
    
    // Simulate walking through time
    for (size_t i = 20; i < all_candles.size(); ++i) {
        // Get a window of data up to current point
        std::vector<Candle> window(all_candles.begin(), all_candles.begin() + i + 1);
        
        // Ask strategy for signal
        Order signal = strategy.on_candles(window, symbol);
        
        if (signal.status != "NONE") {
            std::cout << "\n[Day " << i << "] SIGNAL: " << signal.side 
                     << " @ $" << std::fixed << std::setprecision(2) << signal.price << "\n";
            
            // Execute trade
            Order filled = engine.execute(signal);
            
            std::cout << "         FILLED: " << filled.side 
                     << " " << filled.qty << " shares @ $" << filled.price 
                     << " (status: " << filled.status << ")\n";
            
            double nav = engine.get_cash() + engine.get_position_qty() * signal.price;
            std::cout << "         Portfolio: Cash=$" << engine.get_cash() 
                     << " | Position=" << engine.get_position_qty() 
                     << " | NAV=$" << nav << "\n";
            
            trade_count++;
        }
    }
    
    print_separator();
    std::cout << "SIMULATION COMPLETE\n";
    print_separator();
    
    // Final results
    double final_price = all_candles.back().close;
    double final_cash = engine.get_cash();
    double final_position = engine.get_position_qty();
    double final_nav = final_cash + final_position * final_price;
    double profit = final_nav - 10000.0;
    double profit_pct = (profit / 10000.0) * 100.0;
    
    std::cout << "\n=== FINAL RESULTS ===\n\n";
    std::cout << "Total Trades Executed: " << trade_count << "\n";
    std::cout << "Final Cash: $" << std::fixed << std::setprecision(2) << final_cash << "\n";
    std::cout << "Final Position: " << final_position << " shares @ $" << final_price << "\n";
    std::cout << "Final NAV: $" << final_nav << "\n";
    std::cout << "Profit/Loss: $" << profit << " (" << std::showpos << profit_pct << std::noshowpos << "%)\n";
    
    // Buy & Hold comparison
    double buy_hold_shares = 10000.0 / all_candles[20].close;
    double buy_hold_nav = buy_hold_shares * final_price;
    double buy_hold_profit = buy_hold_nav - 10000.0;
    double buy_hold_pct = (buy_hold_profit / 10000.0) * 100.0;
    
    std::cout << "\n=== BUY & HOLD COMPARISON ===\n\n";
    std::cout << "Buy & Hold NAV: $" << buy_hold_nav << "\n";
    std::cout << "Buy & Hold P/L: $" << buy_hold_profit << " (" << std::showpos << buy_hold_pct << std::noshowpos << "%)\n";
    
    if (profit > buy_hold_profit) {
        std::cout << "\n + Strategy OUTPERFORMED buy & hold by $" << (profit - buy_hold_profit) << "\n";
    } else {
        std::cout << "\n - Strategy underperformed buy & hold by $" << (buy_hold_profit - profit) << "\n";
    }
    
    print_separator();
    
    return 0;
}
