#!/bin/bash

echo " Test 1: Default Config (SPY Daily)"
./build/configurable_backtest 2>&1 | grep -E "(Configuration:|Symbol:|Strategy:|Trades Executed:|Profit/Loss:)"

echo ""
echo " Test 2: Aggressive AAPL Config"
./build/configurable_backtest --config configs/aggressive_aapl.json 2>&1 | grep -E "(Configuration:|Symbol:|Strategy:|Trades Executed:|Profit/Loss:)"

echo ""
echo " Test 3: Crypto Long-term Strategy"
./build/configurable_backtest --config configs/crypto_longterm.json 2>&1 | grep -E "(Configuration:|Symbol:|Strategy:|Trades Executed:|Profit/Loss:)"

echo ""
echo "ALL TESTS COMPLETED."

