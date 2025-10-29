#ifndef PAPERENGINE_H
#define PAPERENGINE_H

#include "order.h"
#include "candle.h"
#include <mutex>
#include <vector>

// Simula execução de ordens, posição e capital.
// Mostra ENCAPSULAMENTO: operações de execução e estado interno ficam aqui.
class PaperEngine {
public:
    PaperEngine(double initial_cash = 10000.0);

    // Tenta executar a ordem e retorna a ordem com status atualizado.
    Order execute(const Order& order);

    double get_cash();
    double get_position_qty();
    double get_avg_price();

    // Histórico de trades
    const std::vector<Order>& get_trades() const;

private:
    double cash;
    double pos_qty;
    double pos_avg_price;
    std::vector<Order> trades;
    std::mutex mtx;
};

#endif // PAPERENGINE_H
