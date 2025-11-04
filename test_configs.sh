#!/bin/bash

echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo " Test 1: Default Config (SPY Daily)"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
./build/configurable_backtest 2>&1 | grep -E "(Configuration:|Symbol:|Strategy:|Trades Executed:|Profit/Loss:)"

echo ""
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo " Test 2: Aggressive AAPL Config"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
./build/configurable_backtest --config configs/aggressive_aapl.json 2>&1 | grep -E "(Configuration:|Symbol:|Strategy:|Trades Executed:|Profit/Loss:)"

echo ""
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo " Test 3: Crypto Long-term Strategy"  
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
./build/configurable_backtest --config configs/crypto_longterm.json 2>&1 | grep -E "(Configuration:|Symbol:|Strategy:|Trades Executed:|Profit/Loss:)"

echo ""
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo " Test 4: Command Line Override (TSLA with custom params)"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
./build/configurable_backtest --symbol TSLA --sma-short 3 --sma-long 15 --qty 5 2>&1 | grep -E "(Configuration:|Symbol:|Strategy:|Trades Executed:|Profit/Loss:)"

echo ""
echo "╔══════════════════════════════════════════════════════════════════╗"
echo "║                    ALL TESTS COMPLETE                            ║"
echo "╚══════════════════════════════════════════════════════════════════╝"
echo ""
echo " Tip: Create your own config file and test with:"
echo "   ./build/configurable_backtest --config my_config.json"
echo ""
echo " See CONFIG_GUIDE.md for full documentation"

