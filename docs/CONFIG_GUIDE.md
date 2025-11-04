# Trading Bot Configuration Guide

## Overview

The trading bot now supports flexible configuration through JSON files and command-line arguments, making it easy to customize strategies, test different assets, and adjust parameters without recompiling.

## Quick Start

### Using Default Config

```bash
./build/configurable_backtest
```

This uses `config.json` in the root directory.

### Using Command Line Arguments

```bash
# Test TSLA with different SMA periods
./build/configurable_backtest --symbol TSLA --sma-short 10 --sma-long 50

# Test BTC-USD from 2023
./build/configurable_backtest --symbol BTC-USD --start 2023-01-01

# Use different capital and quantity
./build/configurable_backtest --symbol AAPL --capital 50000 --qty 10
```

### Using Preset Configs

```bash
# Aggressive Apple trading
./build/configurable_backtest --config configs/aggressive_aapl.json

# Long-term crypto strategy
./build/configurable_backtest --config configs/crypto_longterm.json

# Conservative weekly trading
./build/configurable_backtest --config configs/conservative_weekly.json

# Volatile day-trader style
./build/configurable_backtest --config configs/volatile_daytrader.json
```

## Configuration File Format

### config.json Structure

```json
{
  "data_source": "yahoo",
  "symbol": "SPY",
  "interval": "1d",
  "backtest": {
    "enabled": true,
    "start_date": "2024-01-01",
    "end_date": "2024-11-01",
    "initial_capital": 10000.0
  },
  "strategy": {
    "type": "sma",
    "short_period": 5,
    "long_period": 20,
    "trade_quantity": 1.0
  },
  "live_trading": {
    "poll_interval_seconds": 60,
    "fetch_limit": 200
  }
}
```

### Field Descriptions

| Field | Type | Description | Default | Options |
|-------|------|-------------|---------|---------|
| `data_source` | string | Data provider | `"yahoo"` | `"yahoo"`, `"binance"` |
| `symbol` | string | Trading symbol | `"SPY"` | Any valid symbol for the data source |
| `interval` | string | Candle interval | `"1d"` | `"1d"`, `"1wk"`, `"1mo"` (Yahoo) or `"1m"`, `"5m"`, `"1h"` (Binance) |

#### Backtest Settings

| Field | Type | Description | Default |
|-------|------|-------------|---------|
| `enabled` | boolean | Run backtest mode | `true` |
| `start_date` | string | Start date (YYYY-MM-DD) | `"2024-01-01"` |
| `end_date` | string | End date (YYYY-MM-DD) | `"2024-11-01"` |
| `initial_capital` | number | Starting cash | `10000.0` |

#### Strategy Settings

| Field | Type | Description | Default |
|-------|------|-------------|---------|
| `type` | string | Strategy type | `"sma"` |
| `short_period` | integer | Short SMA period | `5` |
| `long_period` | integer | Long SMA period | `20` |
| `trade_quantity` | number | Shares/units per trade | `1.0` |

#### Live Trading Settings

| Field | Type | Description | Default |
|-------|------|-------------|---------|
| `poll_interval_seconds` | integer | Seconds between polls | `60` |
| `fetch_limit` | integer | Number of candles to fetch | `200` |

## Command Line Options

All config file settings can be overridden via command line:

```
--config FILE          Load configuration from FILE
--symbol SYMBOL        Override symbol (e.g., SPY, AAPL, BTC-USD)
--source SOURCE        Data source: yahoo or binance
--start DATE           Backtest start date YYYY-MM-DD
--end DATE             Backtest end date YYYY-MM-DD
--capital AMOUNT       Initial capital
--sma-short N          Short SMA period
--sma-long N           Long SMA period
--qty AMOUNT           Trade quantity
--interval INTERVAL    Data interval (1d, 1wk, 1mo)
--help                 Show help message
```

## Preset Configurations

### 1. Default (`config.json`)
- **Symbol**: SPY (S&P 500)
- **Period**: 2024 (10 months)
- **Capital**: $10,000
- **Strategy**: SMA(5, 20)
- **Interval**: Daily
- **Use case**: Conservative index fund trading

### 2. Aggressive AAPL (`configs/aggressive_aapl.json`)
- **Symbol**: AAPL
- **Period**: 2024 (10 months)
- **Capital**: $50,000
- **Strategy**: SMA(10, 50) with 10 shares per trade
- **Interval**: Daily
- **Use case**: Higher capital, larger positions on single stock

### 3. Crypto Long-term (`configs/crypto_longterm.json`)
- **Symbol**: BTC-USD
- **Period**: 2023-2024 (2 years)
- **Capital**: $10,000
- **Strategy**: SMA(7, 25) with 0.1 BTC per trade
- **Interval**: Daily
- **Use case**: Long-term cryptocurrency trend following

### 4. Conservative Weekly (`configs/conservative_weekly.json`)
- **Symbol**: SPY
- **Period**: 2020-2024 (5 years)
- **Capital**: $100,000
- **Strategy**: SMA(4, 13) with 10 shares per trade
- **Interval**: Weekly
- **Use case**: Long-term pension/retirement strategy

### 5. Volatile Day-trader (`configs/volatile_daytrader.json`)
- **Symbol**: TSLA
- **Period**: 2024 (10 months)
- **Capital**: $25,000
- **Strategy**: SMA(3, 15) with 5 shares per trade
- **Interval**: Daily
- **Use case**: Active trading on volatile stocks

## Examples

### Example 1: Quick Test Different Assets

```bash
# Test multiple assets with same strategy
for symbol in SPY QQQ AAPL TSLA; do
  echo "Testing $symbol..."
  ./build/configurable_backtest --symbol $symbol --sma-short 5 --sma-long 20
  echo ""
done
```

### Example 2: Parameter Sweep

```bash
# Find optimal SMA periods for AAPL
for short in 3 5 7 10; do
  for long in 15 20 30 50; do
    if [ $short -lt $long ]; then
      echo "Testing SMA($short, $long)..."
      ./build/configurable_backtest --symbol AAPL \
        --sma-short $short --sma-long $long \
        | grep "Profit/Loss:"
    fi
  done
done
```

### Example 3: Custom Config

Create `my_strategy.json`:
```json
{
  "data_source": "yahoo",
  "symbol": "MSFT",
  "interval": "1d",
  "backtest": {
    "start_date": "2023-01-01",
    "end_date": "2024-11-01",
    "initial_capital": 20000.0
  },
  "strategy": {
    "short_period": 8,
    "long_period": 21,
    "trade_quantity": 5.0
  }
}
```

Then run:
```bash
./build/configurable_backtest --config my_strategy.json
```

### Example 4: Override Config File

```bash
# Use base config but test different symbol
./build/configurable_backtest --config configs/aggressive_aapl.json --symbol MSFT

# Use crypto config but different date range
./build/configurable_backtest --config configs/crypto_longterm.json \
  --start 2024-01-01 --end 2024-11-01
```

## Tips for Configuration

### Choosing SMA Periods

- **Fast Trading** (3-15): More trades, responsive to short-term moves
- **Medium** (5-20): Balanced approach, filters some noise
- **Slow Trading** (10-50): Fewer trades, follows major trends

### Capital Allocation

- **Conservative**: Risk 1-2% per trade
- **Moderate**: Risk 3-5% per trade
- **Aggressive**: Risk 5-10% per trade

### Interval Selection

- **Daily**: Good for swing trading, 1-5 day holds
- **Weekly**: Position trading, weeks to months
- **Monthly**: Long-term investing, months to years

### Symbol Selection

| Type | Low Volatility | Medium | High Volatility |
|------|---------------|--------|-----------------|
| **ETF** | SPY, IWM | QQQ | - |
| **Stocks** | JNJ, PG | AAPL, MSFT | TSLA, NVDA |
| **Crypto** | - | ETH-USD | BTC-USD |

## Troubleshooting

### No Trades Executed

**Possible causes:**
1. SMA periods too close (e.g., 5 vs 6)
2. Date range too short
3. Strong trending market (no crossovers)
4. Trade quantity too large for capital

**Solutions:**
- Increase difference between periods (e.g., 5 vs 30)
- Use longer date range (1+ years)
- Try different symbols
- Reduce trade quantity

### All Trades Rejected

**Possible causes:**
1. Insufficient capital for trade quantity
2. Trying to sell without position

**Solutions:**
- Increase `initial_capital`
- Decrease `trade_quantity`
- Check first signal is BUY

## Building Your Own Config

1. **Start with a preset**: Copy a config that matches your style
2. **Adjust symbol**: Pick an asset you understand
3. **Set date range**: Test on historical data you're familiar with
4. **Tune parameters**: Adjust SMA periods based on results
5. **Scale capital**: Set realistic starting capital
6. **Test**: Run backtest and analyze results

## Next Steps

After finding a good configuration:
1. Save it as a named preset
2. Test on different time periods (walk-forward)
3. Try on correlated symbols
4. Add risk management rules
5. Consider implementing in live mode (paper trading first!)
