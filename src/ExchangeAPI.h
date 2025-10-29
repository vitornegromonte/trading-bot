#ifndef EXCHANGEAPI_H
#define EXCHANGEAPI_H

#include <vector>
#include <string>
#include "candle.h"

// Interface abstrata para acesso a mercado.
class ExchangeAPI {
public:
    virtual ~ExchangeAPI() = default;

    // Retorna candles históricos (mais recentes primeiro ou em ordem cronológica).
    virtual std::vector<Candle> fetch_klines(const std::string& symbol,
                                             const std::string& interval,
                                             int limit) = 0;

    // Em produção, aqui haveria métodos para envio de ordens autenticadas, consulta de orders, etc.
};

#endif // EXCHANGEAPI_H
