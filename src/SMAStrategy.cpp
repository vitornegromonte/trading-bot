#include "SMAStrategy.h"
#include <numeric>
#include <cmath>
#include <sstream>

SMAStrategy::SMAStrategy(int short_period, int long_period, double trade_qty)
    : short_p(short_period), long_p(long_period), qty(trade_qty),
      prev_short_ma(NAN), prev_long_ma(NAN) {}

double SMAStrategy::compute_sma(const std::vector<double>& data, int period) {
    if((int)data.size() < period) return NAN;
    double sum = 0.0;
    for(int i = (int)data.size() - period; i < (int)data.size(); ++i) sum += data[i];
    return sum / double(period);
}

Order SMAStrategy::on_candles(const std::vector<Candle>& candles, const std::string& symbol) {
    Order none;
    none.status = "NONE";

    if((int)candles.size() < long_p + 1) return none;

    std::vector<double> closes;
    closes.reserve(candles.size());
    for (const auto &c : candles) closes.push_back(c.close);

    double ma_short = compute_sma(closes, short_p);
    double ma_long = compute_sma(closes, long_p);

    // calcular prev a partir de uma cópia sem o último candle
    std::vector<double> closes_prev = closes;
    closes_prev.pop_back();
    double ma_short_prev = compute_sma(closes_prev, short_p);
    double ma_long_prev = compute_sma(closes_prev, long_p);

    // Atualiza prevs (opcional; não estritamente necessário)
    prev_short_ma = ma_short_prev;
    prev_long_ma = ma_long_prev;

    double last_price = closes.back();

    // Detecta crossover: crossover de alta -> BUY, crossover de baixa -> SELL
    if(!std::isnan(ma_short_prev) && !std::isnan(ma_long_prev)) {
        if(ma_short_prev < ma_long_prev && ma_short > ma_long) {
            Order o;
            o.id = "";
            o.side = "BUY";
            o.qty = qty;
            o.price = last_price;
            o.status = "NEW";
            return o;
        } else if(ma_short_prev > ma_long_prev && ma_short < ma_long) {
            Order o;
            o.id = "";
            o.side = "SELL";
            o.qty = qty; // note: PaperEngine decide quantidade real a vender
            o.price = last_price;
            o.status = "NEW";
            return o;
        }
    }

    return none;
}
