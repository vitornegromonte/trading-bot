#include <iostream>
#include <iomanip>
#include "YahooFinanceAPI.h"
#include "SMAStrategy.h"
#include "paperengine.h"

void run_backtest(const std::string& symbol, const std::string& start, const std::string& end, 
                  int short_ma, int long_ma, double initial_capital) {
    std::cout << "\n" << std::string(80, '=') << std::endl;
    std::cout << "Backtesting " << symbol << " with SMA(" << short_ma << "," << long_ma << ")\n";
    std::cout << std::string(80, '=') << std::endl;
    
    YahooFinanceAPI api;
    api.set_interval(DAILY);
    
    auto candles = api.get_ticker_data(symbol, start, end);
    if (candles.empty()) {
        std::cout << "No data available for " << symbol << "\n";
        return;
    }
    
    std::cout << "Data: " << candles.size() << " days | "
              << "$" << std::fixed << std::setprecision(2) << candles.front().close 
              << " -> $" << candles.back().close << "\n";
    
    SMAStrategy strategy(short_ma, long_ma, 1.0);
    PaperEngine engine(initial_capital);
    
    int trades = 0;
    for (size_t i = long_ma; i < candles.size(); ++i) {
        std::vector<Candle> window(candles.begin(), candles.begin() + i + 1);
        Order signal = strategy.on_candles(window, symbol);
        
        if (signal.status != "NONE") {
            Order filled = engine.execute(signal);
            if (filled.status == "FILLED") {
                trades++;
                std::cout << "  Trade #" << trades << ": " << filled.side 
                         << " @ $" << filled.price << "\n";
            }
        }
    }
    
    double final_price = candles.back().close;
    double nav = engine.get_cash() + engine.get_position_qty() * final_price;
    double pnl = nav - initial_capital;
    double pnl_pct = (pnl / initial_capital) * 100.0;
    
    double bh_shares = initial_capital / candles[long_ma].close;
    double bh_nav = bh_shares * final_price;
    double bh_pnl = bh_nav - initial_capital;
    double bh_pct = (bh_pnl / initial_capital) * 100.0;
    
    std::cout << "\nResults:\n";
    std::cout << "  Trades: " << trades << "\n";
    std::cout << "  Strategy P/L: $" << pnl << " (" << std::showpos << pnl_pct << std::noshowpos << "%)\n";
    std::cout << "  Buy&Hold P/L: $" << bh_pnl << " (" << std::showpos << bh_pct << std::noshowpos << "%)\n";
    
    if (pnl > bh_pnl) {
        std::cout << "  + OUTPERFORMED by $" << (pnl - bh_pnl) << "\n";
    } else {
        std::cout << "  - Underperformed by $" << (bh_pnl - pnl) << "\n";
    }
}

int main() {
    std::cout << "\n";
    std::cout << "YAHOO FINANCE MULTI-ASSET BACKTEST SUITE \n";
    
    double capital = 10000.0;
    std::string start = "2024-01-01";
    std::string end = "2024-11-01";
    
    // Test various assets and strategies
    run_backtest("SPY", start, end, 5, 20, capital);      // S&P 500 ETF
    run_backtest("QQQ", start, end, 5, 20, capital);      // Nasdaq 100 ETF
    run_backtest("AAPL", start, end, 5, 20, capital);     // Apple
    run_backtest("TSLA", start, end, 5, 20, capital);     // Tesla (volatile)
    run_backtest("BTC-USD", start, end, 5, 20, capital);  // Bitcoin
    
    std::cout << "\n" << std::string(80, '=') << std::endl;
    std::cout << "BACKTEST SUITE COMPLETE\n";
    std::cout << std::string(80, '=') << std::endl << "\n";
    
    return 0;
}
