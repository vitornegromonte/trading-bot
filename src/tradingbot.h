#ifndef TRADINGBOT_H
#define TRADINGBOT_H

#include "ExchangeAPI.h"
#include "strategy.h"
#include "paperengine.h"
#include <memory>
#include <string>
#include <vector>

// Classe que orquestra API + Strategy + Engine.
// Demonstra COMPOSIÇÃO (tem um ExchangeAPI, tem uma Strategy, tem um Engine).
class TradingBot {
public:
    TradingBot(std::unique_ptr<ExchangeAPI> api,
               std::unique_ptr<Strategy> strat,
               std::unique_ptr<PaperEngine> engine,
               const std::string& symbol,
               const std::string& interval = "1m",
               int fetch_limit = 200);

    // Loop simples de polling; bloqueante até stop_flag ser true.
    void run(int poll_seconds = 30);

private:
    std::unique_ptr<ExchangeAPI> api_;
    std::unique_ptr<Strategy> strat_;
    std::unique_ptr<PaperEngine> engine_;
    std::string symbol_;
    std::string interval_;
    int fetch_limit_;
    bool stop_requested_;
};

#endif // TRADINGBOT_H
