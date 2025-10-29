#include <iostream>
#include <thread>
#include <memory>
#include <csignal>
#include "BinanceAPI.h"
#include "SMAStrategy.h"
#include "paperengine.h"
#include "tradingbot.h"

static volatile std::sig_atomic_t g_stop = 0;
void sigint_handler(int) { g_stop = 1; }

int main() {
    std::signal(SIGINT, sigint_handler);
    std::cout << "Inicializando Simple Trading Bot (C++)\n";

    // Cria componentes: API concreta (Binance), Strategy concreta (SMA) e PaperEngine
    auto api = std::make_unique<BinanceAPI>();
    auto strat = std::make_unique<SMAStrategy>(10, 30, 0.001);
    auto engine = std::make_unique<PaperEngine>(10000.0);

    TradingBot bot(std::move(api), std::move(strat), std::move(engine), "BTCUSDT", "1m", 200);

    // Rodar em thread separada para possibilitar terminar com CTRL+C
    std::thread t([&bot](){
        bot.run(30);
    });

    // aguardar CTRL+C
    while(!g_stop) std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "SIGINT recebido - solicitando parada...\n";
    // atualmente TradingBot não tem método stop - em melhoria poderíamos adicionar.
    // Neste MVP vamos terminar o processo. Thread será terminada quando o processo sair.
    std::exit(0);
    return 0;
}
