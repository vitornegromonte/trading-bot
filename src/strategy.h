#ifndef STRATEGY_H
#define STRATEGY_H

#include <vector>
#include "candle.h"
#include "order.h"
#include <string>

// Estratégia abstrata: define interface para gerar sinais.
class Strategy {
public:
    virtual ~Strategy() = default;

    // Recebe histórico de candles (ordenados cronologicamente) e retorna uma ordem opcional.
    // Retorna Order com status="NONE" se não houver ação.
    virtual Order on_candles(const std::vector<Candle>& candles, const std::string& symbol) = 0;
};

#endif // STRATEGY_H
