#ifndef BINANCEAPI_H
#define BINANCEAPI_H

#include "ExchangeAPI.h"
#include <string>

// Implementação concreta da ExchangeAPI para Binance (endpoints públicos).
class BinanceAPI : public ExchangeAPI {
public:
    BinanceAPI();
    ~BinanceAPI() override;

    // Implementa fetch_klines usando libcurl + nlohmann::json
    std::vector<Candle> fetch_klines(const std::string& symbol,
                                     const std::string& interval,
                                     int limit) override;
};

#endif // BINANCEAPI_H
