# Guia de Configuração do Trading Bot

## Visão Geral

O trading bot agora suporta configuração flexível através de arquivos JSON e argumentos de linha de comando, facilitando a personalização de estratégias, teste de diferentes ativos e ajuste de parâmetros sem recompilar.

## Início Rápido

### Usando Configuração Padrão

```bash
./build/configurable_backtest
```

Isso usa o arquivo `config.json` no diretório raiz.

### Usando Argumentos de Linha de Comando

```bash
# Testar TSLA com diferentes períodos de SMA
./build/configurable_backtest --symbol TSLA --sma-short 10 --sma-long 50

# Testar BTC-USD a partir de 2023
./build/configurable_backtest --symbol BTC-USD --start 2023-01-01

# Usar capital e quantidade diferentes
./build/configurable_backtest --symbol AAPL --capital 50000 --qty 10
```

### Usando Configurações Pré-definidas

```bash
# Trading agressivo da Apple
./build/configurable_backtest --config configs/aggressive_aapl.json

# Estratégia de longo prazo para crypto
./build/configurable_backtest --config configs/crypto_longterm.json

# Trading conservador semanal
./build/configurable_backtest --config configs/conservative_weekly.json

# Estilo day-trader volátil
./build/configurable_backtest --config configs/volatile_daytrader.json
```

## Formato do Arquivo de Configuração

### Estrutura do config.json

```json
{
  "data_source": "yahoo",
  "symbol": "SPY",
  "interval": "1d",
  "backtest": {
    "enabled": true,
    "start_date": "2024-01-01",
    "end_date": "2024-11-01",
    "initial_capital": 10000.0
  },
  "strategy": {
    "type": "sma",
    "short_period": 5,
    "long_period": 20,
    "trade_quantity": 1.0
  },
  "live_trading": {
    "poll_interval_seconds": 60,
    "fetch_limit": 200
  }
}
```

### Descrição dos Campos

| Campo | Tipo | Descrição | Padrão | Opções |
|-------|------|-----------|--------|---------|
| `data_source` | string | Provedor de dados | `"yahoo"` | `"yahoo"`, `"binance"` |
| `symbol` | string | Símbolo de negociação | `"SPY"` | Qualquer símbolo válido |
| `interval` | string | Intervalo de candles | `"1d"` | `"1d"`, `"1wk"`, `"1mo"` (Yahoo) |

#### Configurações de Backtest

| Campo | Tipo | Descrição | Padrão |
|-------|------|-----------|--------|
| `enabled` | boolean | Executar modo backtest | `true` |
| `start_date` | string | Data inicial (YYYY-MM-DD) | `"2024-01-01"` |
| `end_date` | string | Data final (YYYY-MM-DD) | `"2024-11-01"` |
| `initial_capital` | number | Capital inicial | `10000.0` |

#### Configurações de Estratégia

| Campo | Tipo | Descrição | Padrão |
|-------|------|-----------|--------|
| `type` | string | Tipo de estratégia | `"sma"` |
| `short_period` | integer | Período da SMA curta | `5` |
| `long_period` | integer | Período da SMA longa | `20` |
| `trade_quantity` | number | Ações/unidades por operação | `1.0` |

#### Configurações de Trading ao Vivo

| Campo | Tipo | Descrição | Padrão |
|-------|------|-----------|--------|
| `poll_interval_seconds` | integer | Segundos entre consultas | `60` |
| `fetch_limit` | integer | Número de candles a buscar | `200` |

## Opções de Linha de Comando

Todas as configurações podem ser sobrescritas via linha de comando:

```
--config ARQUIVO       Carregar configuração do ARQUIVO
--symbol SÍMBOLO       Sobrescrever símbolo (ex: SPY, AAPL, BTC-USD)
--source FONTE         Fonte de dados: yahoo ou binance
--start DATA           Data inicial do backtest YYYY-MM-DD
--end DATA             Data final do backtest YYYY-MM-DD
--capital VALOR        Capital inicial
--sma-short N          Período da SMA curta
--sma-long N           Período da SMA longa
--qty QUANTIDADE       Quantidade por operação
--interval INTERVALO   Intervalo de dados (1d, 1wk, 1mo)
--help                 Mostrar mensagem de ajuda
```

## Configurações Pré-definidas

### 1. Padrão (`config.json`)
- **Símbolo**: SPY (S&P 500)
- **Período**: 2024 (10 meses)
- **Capital**: $10.000
- **Estratégia**: SMA(5, 20)
- **Intervalo**: Diário

### 2. AAPL Agressivo (`configs/aggressive_aapl.json`)
- **Símbolo**: AAPL
- **Capital**: $50.000
- **Estratégia**: SMA(10, 50) com 10 ações por operação

### 3. Crypto Longo Prazo (`configs/crypto_longterm.json`)
- **Símbolo**: BTC-USD
- **Período**: 2023-2024 (2 anos)
- **Estratégia**: SMA(7, 25) com 0.1 BTC por operação

### 4. Conservador Semanal (`configs/conservative_weekly.json`)
- **Símbolo**: SPY
- **Período**: 2020-2024 (5 anos)
- **Capital**: $100.000
- **Intervalo**: Semanal

### 5. Day-trader Volátil (`configs/volatile_daytrader.json`)
- **Símbolo**: TSLA
- **Capital**: $25.000
- **Estratégia**: SMA(3, 15) - trading ativo

## Exemplos

### Exemplo 1: Teste Rápido de Diferentes Ativos

```bash
# Testar múltiplos ativos com mesma estratégia
for symbol in SPY QQQ AAPL TSLA; do
  echo "Testando $symbol..."
  ./build/configurable_backtest --symbol $symbol --sma-short 5 --sma-long 20
  echo ""
done
```

### Exemplo 2: Varredura de Parâmetros

```bash
# Encontrar períodos ótimos de SMA para AAPL
for short in 3 5 7 10; do
  for long in 15 20 30 50; do
    if [ $short -lt $long ]; then
      echo "Testando SMA($short, $long)..."
      ./build/configurable_backtest --symbol AAPL \
        --sma-short $short --sma-long $long \
        | grep "Lucro/Prejuízo:"
    fi
  done
done
```

### Exemplo 3: Configuração Personalizada

Crie `minha_estrategia.json`:
```json
{
  "data_source": "yahoo",
  "symbol": "MSFT",
  "interval": "1d",
  "backtest": {
    "start_date": "2023-01-01",
    "end_date": "2024-11-01",
    "initial_capital": 20000.0
  },
  "strategy": {
    "short_period": 8,
    "long_period": 21,
    "trade_quantity": 5.0
  }
}
```

Execute:
```bash
./build/configurable_backtest --config minha_estrategia.json
```

### Exemplo 4: Sobrescrever Arquivo de Configuração

```bash
# Usar config base mas testar símbolo diferente
./build/configurable_backtest --config configs/aggressive_aapl.json --symbol MSFT

# Usar config crypto mas período diferente
./build/configurable_backtest --config configs/crypto_longterm.json \
  --start 2024-01-01 --end 2024-11-01
```

## Dicas para Configuração

### Escolhendo Períodos de SMA

- **Trading Rápido** (3-15): Mais operações, responsivo a movimentos de curto prazo
- **Médio** (5-20): Abordagem balanceada, filtra algum ruído
- **Trading Lento** (10-50): Menos operações, segue tendências maiores

### Alocação de Capital

- **Conservador**: Arriscar 1-2% por operação
- **Moderado**: Arriscar 3-5% por operação
- **Agressivo**: Arriscar 5-10% por operação

### Seleção de Intervalo

- **Diário**: Bom para swing trading, manter 1-5 dias
- **Semanal**: Position trading, semanas a meses
- **Mensal**: Investimento de longo prazo, meses a anos

### Seleção de Símbolo

| Tipo | Baixa Volatilidade | Média | Alta Volatilidade |
|------|-------------------|-------|-------------------|
| **ETF** | SPY, IWM | QQQ | - |
| **Ações** | JNJ, PG | AAPL, MSFT | TSLA, NVDA |
| **Crypto** | - | ETH-USD | BTC-USD |

## Solução de Problemas

### Nenhuma Operação Executada

**Possíveis causas:**
1. Períodos de SMA muito próximos (ex: 5 vs 6)
2. Intervalo de datas muito curto
3. Mercado em forte tendência (sem cruzamentos)
4. Quantidade de operação muito grande para o capital

**Soluções:**
- Aumentar diferença entre períodos (ex: 5 vs 30)
- Usar intervalo de datas maior (1+ anos)
- Testar símbolos diferentes
- Reduzir quantidade de operação

### Todas as Operações Rejeitadas

**Possíveis causas:**
1. Capital insuficiente para quantidade de operação
2. Tentando vender sem posição

**Soluções:**
- Aumentar `initial_capital`
- Diminuir `trade_quantity`
- Verificar se primeiro sinal é COMPRA

## Construindo Sua Própria Configuração

1. **Comece com um preset**: Copie uma config que combine com seu estilo
2. **Ajuste o símbolo**: Escolha um ativo que você entende
3. **Defina intervalo de datas**: Teste em dados históricos familiares
4. **Ajuste parâmetros**: Modifique períodos de SMA baseado nos resultados
5. **Escale capital**: Defina capital inicial realista
6. **Teste**: Execute backtest e analise resultados

## Próximos Passos

Após encontrar uma boa configuração:
1. Salve-a como um preset nomeado
2. Teste em diferentes períodos de tempo
3. Experimente em símbolos correlacionados
4. Adicione regras de gerenciamento de risco
5. Considere implementar em modo ao vivo (paper trading primeiro!)
