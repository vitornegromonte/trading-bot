#!/bin/bash

echo "\n"
echo "CUSTOM CONFIGURATION GENERATOR"
echo "\n"

read -p "Symbol (e.g., AAPL, SPY, BTC-USD): " SYMBOL
read -p "Start date (YYYY-MM-DD) [2024-01-01]: " START
START=${START:-2024-01-01}
read -p "End date (YYYY-MM-DD) [2024-11-01]: " END
END=${END:-2024-11-01}
read -p "Initial capital [10000]: " CAPITAL
CAPITAL=${CAPITAL:-10000}
read -p "Short SMA period [5]: " SHORT
SHORT=${SHORT:-5}
read -p "Long SMA period [20]: " LONG
LONG=${LONG:-20}
read -p "Trade quantity [1.0]: " QTY
QTY=${QTY:-1.0}

FILENAME="configs/custom_${SYMBOL}_$(date +%Y%m%d).json"

cat > "$FILENAME" << CONFIGEOF
{
  "data_source": "yahoo",
  "symbol": "$SYMBOL",
  "interval": "1d",
  "backtest": {
    "enabled": true,
    "start_date": "$START",
    "end_date": "$END",
    "initial_capital": $CAPITAL
  },
  "strategy": {
    "type": "sma",
    "short_period": $SHORT,
    "long_period": $LONG,
    "trade_quantity": $QTY
  },
  "live_trading": {
    "poll_interval_seconds": 60,
    "fetch_limit": 200
  }
}
CONFIGEOF

echo ""
echo "Configuration saved to: $FILENAME"
echo ""
echo "Test it with:"
echo "   ./build/configurable_backtest --config $FILENAME"
echo ""

read -p "Run backtest now? (y/n) " RUN
if [ "$RUN" = "y" ] || [ "$RUN" = "Y" ]; then
  echo ""
  ./build/configurable_backtest --config "$FILENAME"
fi

