#ifndef CANDLE_H
#define CANDLE_H

#include <string>
#include <cstdint>

// Representa um candle OHLCV
struct Candle {
    int64_t ts_ms;   // timestamp em ms
    double open;
    double high;
    double low;
    double close;
    double volume;

    Candle() = default;
    Candle(int64_t ts, double o, double h, double l, double c, double v)
        : ts_ms(ts), open(o), high(h), low(l), close(c), volume(v) {}
};

#endif // CANDLE_H
