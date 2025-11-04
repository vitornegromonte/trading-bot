# 📚 Referência Rápida

## Executáveis Disponíveis

### 1. Backtest Configurável
```bash
./build/configurable_backtest --config config.json
./build/configurable_backtest --symbol AAPL --start 2023-01-01 --end 2024-01-01
```

### 2. Backtest Yahoo Detalhado
```bash
./build/backtest_yahoo
```
Executa backtest detalhado com SPY mostrando todas as operações.

### 3. Multi-Backtest
```bash
./build/multi_backtest
```
Compara múltiplos ativos (SPY, QQQ, AAPL, TSLA, BTC-USD).

## Parâmetros de Linha de Comando

| Parâmetro | Descrição | Exemplo |
|-----------|-----------|---------|
| `--config` | Arquivo de configuração JSON | `--config configs/aggressive.json` |
| `--symbol` | Símbolo do ativo | `--symbol AAPL` |
| `--start` | Data inicial (YYYY-MM-DD) | `--start 2023-01-01` |
| `--end` | Data final (YYYY-MM-DD) | `--end 2024-01-01` |
| `--capital` | Capital inicial | `--capital 100000` |
| `--sma-short` | Período da SMA curta | `--sma-short 10` |
| `--sma-long` | Período da SMA longa | `--sma-long 50` |
| `--qty` | Quantidade por operação | `--qty 10` |
| `--interval` | Intervalo (daily/weekly/monthly) | `--interval weekly` |

## Estrutura do JSON de Configuração

```json
{
  "backtest": {
    "symbol": "AAPL",
    "start_date": "2023-01-01",
    "end_date": "2024-01-01",
    "initial_capital": 10000.0,
    "interval": "daily"
  },
  "strategy": {
    "type": "SMA",
    "short_period": 10,
    "long_period": 50,
    "quantity": 10
  }
}
```

## APIs Suportadas

### Yahoo Finance
- **Endpoint**: `https://query1.finance.yahoo.com/v8/finance/chart/{symbol}`
- **Intervalos**: daily, weekly, monthly
- **Dados**: OHLCV históricos

### Binance
- **Endpoint**: `https://api.binance.com/api/v3/klines`
- **Intervalos**: 1m, 5m, 15m, 1h, 4h, 1d, 1w, 1M
- **Dados**: OHLCV em tempo real

## Estratégias Disponíveis

### SMA Crossover (Simple Moving Average)
- **Sinal de Compra**: SMA curta cruza acima da SMA longa
- **Sinal de Venda**: SMA curta cruza abaixo da SMA longa
- **Parâmetros**: 
  - `short_period`: Período da média móvel curta (ex: 10)
  - `long_period`: Período da média móvel longa (ex: 50)

## Compilação

```bash
# Compilar tudo
cmake -B build
cmake --build build

# Compilar apenas um executável
cmake --build build --target configurable_backtest
```

## Exemplos Rápidos

### Backtest Agressivo
```bash
./build/configurable_backtest \
  --symbol TSLA \
  --start 2023-01-01 \
  --end 2024-01-01 \
  --capital 50000 \
  --sma-short 5 \
  --sma-long 20 \
  --qty 50
```

### Backtest Conservador
```bash
./build/configurable_backtest \
  --symbol SPY \
  --start 2023-01-01 \
  --end 2024-01-01 \
  --capital 100000 \
  --sma-short 50 \
  --sma-long 200 \
  --qty 10 \
  --interval weekly
```

### Usando Arquivo de Configuração
```bash
./build/configurable_backtest --config configs/crypto_longterm.json
```
