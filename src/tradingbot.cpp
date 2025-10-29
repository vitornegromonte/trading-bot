#include "tradingbot.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>

TradingBot::TradingBot(std::unique_ptr<ExchangeAPI> api,
                       std::unique_ptr<Strategy> strat,
                       std::unique_ptr<PaperEngine> engine,
                       const std::string& symbol,
                       const std::string& interval,
                       int fetch_limit)
    : api_(std::move(api)),
      strat_(std::move(strat)),
      engine_(std::move(engine)),
      symbol_(symbol),
      interval_(interval),
      fetch_limit_(fetch_limit),
      stop_requested_(false) {}

void TradingBot::run(int poll_seconds) {
    using namespace std::chrono_literals;
    std::cout << "Bot iniciado - símbolo: " << symbol_ << " interval: " << interval_ << "\n";

    // loop principal
    while(!stop_requested_) {
        try {
            auto candles = api_->fetch_klines(symbol_, interval_, fetch_limit_);
            if(candles.empty()) {
                std::cerr << "Nenhum candle retornado\n";
            } else {
                // Strategy pode retornar ordem
                Order o = strat_->on_candles(candles, symbol_);
                if(o.status != "NONE") {
                    std::cout << "[SINAL] " << o.side << " qty=" << o.qty << " price=" << o.price << "\n";
                    auto filled = engine_->execute(o);
                    std::cout << "[EXEC] status=" << filled.status << " qty=" << filled.qty << " price=" << filled.price << "\n";
                } else {
                    std::cout << "[SINAL] sem ação no ciclo\n";
                }

                // Print simples de P&L
                double last_price = candles.back().close;
                double nav = engine_->get_cash() + engine_->get_position_qty() * last_price;
                std::cout << "[PORTFOLIO] cash=" << engine_->get_cash()
                          << " pos_qty=" << engine_->get_position_qty()
                          << " nav=" << nav << "\n";
            }
        } catch (const std::exception& ex) {
            std::cerr << "Erro no ciclo: " << ex.what() << "\n";
        }

        for(int i=0;i<poll_seconds;i++) {
            std::this_thread::sleep_for(1s);
            // permitir futura extensão para stop_requested_ = true via sinal/flag
        }
    }

    std::cout << "Bot finalizado\n";
}
