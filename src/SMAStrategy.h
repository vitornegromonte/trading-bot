#ifndef SMA_STRATEGY_H
#define SMA_STRATEGY_H

#include "strategy.h"
#include <deque>

// Implementação simples de crossover de duas SMAs.
class SMAStrategy : public Strategy {
public:
    SMAStrategy(int short_period = 10, int long_period = 30, double trade_qty = 0.001);

    Order on_candles(const std::vector<Candle>& candles, const std::string& symbol) override;

private:
    int short_p;
    int long_p;
    double qty;
    // Guardar estado anterior para detectar crossover
    double prev_short_ma;
    double prev_long_ma;

    double compute_sma(const std::vector<double>& data, int period);
};

#endif // SMA_STRATEGY_H
