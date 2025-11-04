# Trading Bot

[Documentação](https://vitornegromonte.github.io/trading-bot/docs/reference.html) | [Relatório]() | [Vídeo]()


Projeto de um Trading Bot de ações e crypto utilizando APIs da Binance e da Yahoo Finance. Projeto para avaliação da disciplina CIN0135 - Estrutura de Dados Orientadas à Objetos.

## Características

- **Múltiplas Fontes de Dados**: Binance (tempo real) e Yahoo Finance (dados históricos diários/semanais/mensais)
- **Padrão de Estratégia Plugável**: Implemente estratégias de trading customizadas via interface `Strategy`
- **Motor de Paper Trading**: Teste estratégias com backtesting sem risco de capital real
- **Suporte a Intervalos**: Dados DAILY, WEEKLY, MONTHLY do Yahoo Finance
- **Sistema Configurável**: Configure via arquivos JSON ou argumentos de linha de comando
- **Múltiplos Backtests**: Execute testes em múltiplos ativos simultaneamente

## Compilação

```bash
cmake -S . -B build
cmake --build build
```

## Uso

### Backtest Configurável (Recomendado)

A maneira mais fácil de testar estratégias com parâmetros customizados:

```bash
# Usar config.json padrão
./build/configurable_backtest

# Sobrescrever com opções de linha de comando
./build/configurable_backtest --symbol AAPL --sma-short 10 --sma-long 50

# Usar configurações pré-definidas
./build/configurable_backtest --config configs/aggressive_aapl.json
./build/configurable_backtest --config configs/crypto_longterm.json
./build/configurable_backtest --config configs/conservative_weekly.json

# Mostrar todas as opções
./build/configurable_backtest --help
```

**Veja [CONFIG_GUIDE.md](CONFIG_GUIDE.md) para documentação completa de configuração.**

### Executar com Binance (padrão - candles de 1 minuto)

```bash
./build/CInTB
```

### Executar com Yahoo Finance (dados diários)

```bash
./build/CInTB --source=yahoo
```

### Backtest de Estratégias de Trading

Execute um backtest detalhado no SPY (S&P 500) com a estratégia SMA:

```bash
./build/backtest_yahoo
```

Exemplo de saída:
- Mostra cada sinal e execução de trade
- Exibe o status do portfólio após cada trade
- Compara performance da estratégia vs buy & hold
- Reporta P&L final e contagem de trades

Execute backtests em múltiplos ativos (SPY, QQQ, AAPL, TSLA, BTC-USD):

```bash
./build/multi_backtest
```

Isso executa a estratégia SMA(5,20) em diferentes classes de ativos e mostra resultados comparativos.

### Demo do Yahoo Finance API

Uma demonstração standalone mostrando todas as capacidades da API do Yahoo Finance:

```bash
./build/yahoo_demo
```

Esta demo irá:
- Buscar dados diários do SPY
- Buscar dados semanais do BTC-USD
- Baixar dados mensais do AAPL para arquivo JSON
- Usar a interface fetch_klines para TSLA

## API do Yahoo Finance

A classe `YahooFinanceAPI` fornece uma interface completa para dados do Yahoo Finance:

### Métodos

```cpp
// Definir o intervalo de dados
void set_interval(Interval interval);  // DAILY, WEEKLY ou MONTHLY

// Obter candles para um intervalo de datas específico
std::vector<Candle> get_ticker_data(
    const std::string& ticker,
    const std::string& start_date,  // Formato: "YYYY-MM-DD"
    const std::string& end_date
);

// Baixar dados JSON brutos para arquivo
std::string download_ticker_csv(
    const std::string& ticker,
    const std::string& start_date,
    const std::string& end_date
);

// Interface compatível com ExchangeAPI
std::vector<Candle> fetch_klines(
    const std::string& symbol,
    const std::string& interval,  // "1d", "1wk", "1mo"
    int limit
);
```

### Símbolos Suportados

Yahoo Finance suporta:
- Ações: `AAPL`, `TSLA`, `MSFT`
- ETFs: `SPY`, `QQQ`, `IWM`
- Crypto: `BTC-USD`, `ETH-USD`
- Forex: `EURUSD=X`, `GBPUSD=X`

### Intervalos Suportados

- `DAILY` ou `"1d"`: Candles diários
- `WEEKLY` ou `"1wk"`: Candles semanais
- `MONTHLY` ou `"1mo"`: Candles mensais

**Nota**: A API histórica do Yahoo Finance não suporta intervalos intraday (1m, 5m, etc.) para a maioria dos símbolos.

## Fontes de Dados

| Fonte | Símbolos | Intervalos | Tempo Real |
|-------|----------|------------|------------|
| Binance | Pares de crypto (BTCUSDT, ETHUSDT) | 1m, 5m, 1h, 1d, etc. | Sim |
| Yahoo Finance | Ações, ETFs, Crypto, Forex | 1d, 1wk, 1mo | Não (atrasado) |

## Arquitetura

- **ExchangeAPI**: Interface abstrata para fontes de dados de mercado
- **BinanceAPI**: Implementação para a exchange Binance
- **YahooFinanceAPI**: Implementação para Yahoo Finance
- **Strategy**: Interface abstrata para estratégias de trading
- **SMAStrategy**: Estratégia de cruzamento de Médias Móveis Simples
- **PaperEngine**: Motor de execução de ordens simuladas
- **TradingBot**: Orquestrador principal que busca dados e executa estratégias

## Configuração

O bot suporta configuração flexível através de arquivos JSON e argumentos de linha de comando:

### Opções de Configuração

| Parâmetro | Descrição | Padrão |
|-----------|-----------|--------|
| `--symbol` | Símbolo para negociar (SPY, AAPL, BTC-USD) | SPY |
| `--source` | Fonte de dados (yahoo, binance) | yahoo |
| `--interval` | Intervalo de candles (1d, 1wk, 1mo) | 1d |
| `--start` | Data de início (YYYY-MM-DD) | 2024-01-01 |
| `--end` | Data de término (YYYY-MM-DD) | 2024-11-01 |
| `--capital` | Capital inicial | 10000 |
| `--sma-short` | Período SMA curto | 5 |
| `--sma-long` | Período SMA longo | 20 |
| `--qty` | Quantidade por trade | 1.0 |

### Configurações Pré-definidas

O projeto inclui várias configurações prontas para uso em `configs/`:

- **aggressive_aapl.json**: Trading agressivo em AAPL com capital de $50k
- **crypto_longterm.json**: Estratégia de 2 anos em BTC-USD
- **conservative_weekly.json**: Trading semanal conservador em SPY
- **volatile_daytrader.json**: Trading ativo em TSLA

### Exemplos de Uso

```bash
# Testar TSLA com SMA customizado
./build/configurable_backtest --symbol TSLA --sma-short 10 --sma-long 50

# Usar preset de crypto
./build/configurable_backtest --config configs/crypto_longterm.json

# Criar configuração personalizada
./create_custom_config.sh

# Testar todas as configurações pré-definidas
./test_configs.sh
```

## Documentação Adicional

- **CONFIG_GUIDE.md**: Guia completo de configuração
- **TRADING_GUIDE.md**: Explicação do sistema de trading
- **QUICK_REFERENCE.md**: Referência rápida da API
- **YAHOO_API_SUMMARY.md**: Resumo da integração com Yahoo Finance

## Dependências

- CMake 3.10+
- C++17
- libcurl
- nlohmann/json 3.2.0+

## Estrutura do Projeto

```
trading-bot/
├── src/
│   ├── main.cpp                     # Aplicação principal
│   ├── BinanceAPI.{h,cpp}           # Implementação da API Binance
│   ├── YahooFinanceAPI.{h,cpp}      # Implementação da API Yahoo Finance
│   ├── ExchangeAPI.h                # Interface abstrata
│   ├── SMAStrategy.{h,cpp}          # Estratégia SMA
│   ├── paperengine.{h,cpp}          # Motor de simulação
│   ├── tradingbot.{h,cpp}           # Orquestrador principal
│   ├── config.h                     # Sistema de configuração
│   ├── interval.h                   # Definição de intervalos
│   ├── candle.h                     # Estrutura de dados de candle
│   ├── order.h                      # Estrutura de ordem
│   ├── strategy.h                   # Interface de estratégia
│   ├── backtest_yahoo.cpp           # Backtest detalhado
│   ├── multi_backtest.cpp           # Backtest multi-ativos
│   ├── configurable_backtest.cpp    # Backtest configurável
│   └── yahoo_demo.cpp               # Demo da API
├── configs/                         # Configurações pré-definidas
├── examples/                        # Exemplos de código
├── CMakeLists.txt                   # Configuração de build
├── config.json                      # Configuração padrão
└── README.md                        # Este arquivo

```

## Licença

Este projeto foi desenvolvido para fins educacionais como parte da disciplina CIN0135 - Estrutura de Dados Orientadas à Objetos.


