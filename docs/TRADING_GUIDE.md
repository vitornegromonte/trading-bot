# Trading Bot - Complete Guide

## Quick Start

### 1. Build Everything
```bash
cd /home/vitor/development/trading-bot
cmake -S . -B build
cmake --build build
```

### 2. Run Backtests (See Trading Results)

#### Single Asset Detailed Backtest
```bash
./build/backtest_yahoo
```

This shows:
- ✅ Every trade signal (BUY/SELL)
- ✅ Execution prices and quantities
- ✅ Portfolio value after each trade
- ✅ Final P&L and performance metrics
- ✅ Comparison with buy & hold strategy

**Example Output:**
```
[Day 86] SIGNAL: BUY @ $516.57
         FILLED: BUY 1.00 shares @ $516.57 (status: FILLED)
         Portfolio: Cash=$9483.43 | Position=1.00 | NAV=$10000.00

[Day 140] SIGNAL: SELL @ $541.23
         FILLED: SELL 1.00 shares @ $541.23 (status: FILLED)
         Portfolio: Cash=$10024.66 | Position=0.00 | NAV=$10024.66

=== FINAL RESULTS ===
Total Trades Executed: 7
Final NAV: $10017.75
Profit/Loss: $17.75 (+0.18%)
```

#### Multi-Asset Backtest Suite
```bash
./build/multi_backtest
```

Tests the strategy on:
- SPY (S&P 500 ETF)
- QQQ (Nasdaq 100)
- AAPL (Apple Stock)
- TSLA (Tesla - high volatility)
- BTC-USD (Bitcoin)

Shows comparative performance across different asset classes.

## What's Connected

### ✅ Yahoo Finance API → Trading Bot
The bot is **fully connected** to Yahoo Finance data. Here's what works:

1. **Data Fetching** ✅
   - Fetches daily OHLCV data for any symbol
   - Supports stocks, ETFs, crypto, forex
   - Handles date ranges properly

2. **Strategy Execution** ✅
   - SMA(5, 20) crossover strategy
   - Generates BUY/SELL signals based on moving average crosses
   - Detects uptrend/downtrend reversals

3. **Trade Execution** ✅
   - Paper trading engine simulates order fills
   - Tracks cash and position inventory
   - Calculates portfolio NAV

4. **Performance Tracking** ✅
   - Counts total trades
   - Calculates profit/loss
   - Compares with buy & hold benchmark

## Why The Live Bot Shows "No Action"

When you run `./build/simple_trading_bot --source=yahoo`, you see:
```
[SINAL] sem ação no ciclo
```

This is **expected behavior** because:

1. **Daily data doesn't change every 30 seconds** - The bot polls every 30 seconds, but Yahoo only has daily candles that update once per day

2. **Need historical window to generate signals** - The SMA strategy needs to see price changes over time to detect crossovers

3. **Live mode vs Backtest mode**:
   - **Live mode**: Waits for new data (slow with daily candles)
   - **Backtest mode**: Replays historical data (fast, shows all trades)

## How to See Trading Activity

### Option 1: Run Backtest (Recommended)
```bash
./build/backtest_yahoo
```
This replays 2024 data day-by-day and shows all trades.

### Option 2: Run Multi-Asset Backtest
```bash
./build/multi_backtest
```
This tests 5 different assets and shows which ones generated trades.

### Option 3: Live Mode (Slow)
You would need to:
- Run the bot continuously for days/weeks
- Wait for actual crossover signals to occur
- Use `--source=binance` for faster 1-minute updates

## Understanding the Results

### What the Backtest Shows

**Example from SPY backtest:**
- Started with: $10,000
- Made 6 trades over 211 days
- Ended with: $10,017.75
- Profit: $17.75 (+0.18%)
- Buy & Hold would have made: $1,776.01 (+17.76%)

**Why strategy underperformed:**
- Simple SMA crossover is **too slow** in trending markets
- Works better in **sideways/choppy** markets
- Many **whipsaw trades** (buy high, sell low during corrections)

### Most Active Trading

From the results, **TSLA** generated the most trades (17) due to high volatility:
```
Trade #1: BUY @ $199.95
Trade #2: SELL @ $176.54   <- Loss
Trade #3: BUY @ $175.22
Trade #4: SELL @ $171.11   <- Loss
...
Trade #16: SELL @ $241.05  <- Profit
Trade #17: BUY @ $262.51
```

## Executables Summary

| Executable | Purpose | What it shows |
|------------|---------|---------------|
| `simple_trading_bot` | Live trading mode | Waits for real-time signals (slow with daily data) |
| `backtest_yahoo` | Single detailed backtest | Full trade log for SPY with SMA(5,20) |
| `multi_backtest` | Multi-asset backtest | Quick results for 5 different assets |
| `yahoo_demo` | API testing | Shows data fetching capabilities |

## Next Steps to Improve Trading

1. **Tune Strategy Parameters**
   - Try different SMA periods (e.g., SMA(10, 50))
   - Add momentum filters
   - Use RSI or MACD instead

2. **Better Risk Management**
   - Add stop-loss orders
   - Position sizing based on volatility
   - Don't trade during low-volume periods

3. **Test Different Markets**
   - Find assets with mean-reverting behavior
   - Avoid strongly trending markets for SMA crossover

4. **Add More Strategies**
   - Mean reversion
   - Breakout strategies
   - Pair trading

## Files Created for Trading

- `src/backtest_yahoo.cpp` - Detailed single-asset backtest
- `src/multi_backtest.cpp` - Multi-asset comparison backtest
- `src/YahooFinanceAPI.cpp` - Yahoo Finance data connector
- `src/SMAStrategy.cpp` - Moving average crossover strategy
- `src/paperengine.cpp` - Simulated trade execution

All components are **connected and working**. The backtests prove the entire pipeline works end-to-end! 🎉
