#include <iostream>
#include <thread>
#include <memory>
#include <csignal>
#include "BinanceAPI.h"
#include "YahooFinanceAPI.h"
#include "SMAStrategy.h"
#include "paperengine.h"
#include "tradingbot.h"

static volatile std::sig_atomic_t g_stop = 0;
void sigint_handler(int) { g_stop = 1; }

int main(int argc, char** argv) {
    std::signal(SIGINT, sigint_handler);
    std::cout << "Inicializando Simple Trading Bot (C++)\n";

    // Choose data source with argument: --source=binance (default) or --source=yahoo
    std::string source = "binance";
    for (int i = 1; i < argc; ++i) {
        std::string a = std::string(argv[i]);
        if (a.rfind("--source=", 0) == 0) source = a.substr(9);
        else if (a == "-s" && i+1 < argc) { source = std::string(argv[++i]); }
    }

    std::unique_ptr<ExchangeAPI> api;
    if (source == "yahoo") {
        api = std::make_unique<YahooFinanceAPI>();
    } else {
        api = std::make_unique<BinanceAPI>();
    }
    auto strat = std::make_unique<SMAStrategy>(10, 30, 0.001);
    auto engine = std::make_unique<PaperEngine>(10000.0);

    std::string symbol = (source == "yahoo") ? "BTC-USD" : "BTCUSDT";
    std::string interval = (source == "yahoo") ? "1d" : "1m";
    TradingBot bot(std::move(api), std::move(strat), std::move(engine), symbol, interval, 200);

    // Run in a separate thread to allow termination with CTRL+C
    std::thread t([&bot](){
        bot.run(30);
    });

    // Wait for CTRL+C
    while(!g_stop) std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "SIGINT received - requesting stop...\n";
    // Currently TradingBot does not have a stop method - this could be an improvement.
    std::exit(0);
    return 0;
}
