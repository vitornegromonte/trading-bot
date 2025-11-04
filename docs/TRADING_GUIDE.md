# Trading Bot - Guia Completo

## Início Rápido

### 1. Compilar Tudo
```bash
cd /home/vitor/development/trading-bot
cmake -S . -B build
cmake --build build
```

### 2. Executar Backtests (Ver Resultados de Trading)

#### Backtest Detalhado de Ativo Único
```bash
./build/backtest_yahoo
```

Isso mostra:
- ✅ Cada sinal de operação (COMPRA/VENDA)
- ✅ Preços e quantidades de execução
- ✅ Valor do portfólio após cada operação
- ✅ Lucro/Prejuízo final e métricas de performance
- ✅ Comparação com estratégia buy & hold

**Exemplo de Saída:**
```
[Dia 86] SINAL: COMPRA @ $516.57
         EXECUTADO: COMPRA 1.00 ações @ $516.57 (status: FILLED)
         Portfólio: Dinheiro=$9483.43 | Posição=1.00 | NAV=$10000.00

[Dia 140] SINAL: VENDA @ $541.23
         EXECUTADO: VENDA 1.00 ações @ $541.23 (status: FILLED)
         Portfólio: Dinheiro=$10024.66 | Posição=0.00 | NAV=$10024.66

=== RESULTADOS FINAIS ===
Total de Operações Executadas: 7
NAV Final: $10017.75
Lucro/Prejuízo: $17.75 (+0.18%)
```

#### Suite de Backtest Multi-Ativos
```bash
./build/multi_backtest
```

Testa a estratégia em:
- SPY (ETF do S&P 500)
- QQQ (Nasdaq 100)
- AAPL (Ações da Apple)
- TSLA (Tesla - alta volatilidade)
- BTC-USD (Bitcoin)

Mostra performance comparativa entre diferentes classes de ativos.

## O Que Está Conectado

### ✅ API do Yahoo Finance → Trading Bot
O bot está **totalmente conectado** aos dados do Yahoo Finance. Aqui está o que funciona:

1. **Busca de Dados** ✅
   - Busca dados OHLCV diários para qualquer símbolo
   - Suporta ações, ETFs, crypto, forex
   - Lida com intervalos de datas adequadamente

2. **Execução de Estratégia** ✅
   - Estratégia de cruzamento SMA(5, 20)
   - Gera sinais de COMPRA/VENDA baseados em cruzamentos de médias móveis
   - Detecta reversões de tendência de alta/baixa

3. **Execução de Operações** ✅
   - Engine de paper trading simula preenchimento de ordens
   - Rastreia dinheiro e inventário de posições
   - Calcula NAV do portfólio

4. **Rastreamento de Performance** ✅
   - Conta total de operações
   - Calcula lucro/prejuízo
   - Compara com benchmark buy & hold

## Por Que o Bot ao Vivo Mostra "Sem Ação"

Quando você executa `./build/simple_trading_bot --source=yahoo`, você vê:
```
[SINAL] sem ação no ciclo
```

Esse é um **comportamento esperado** porque:

1. **Dados diários não mudam a cada 30 segundos** - O bot consulta a cada 30 segundos, mas Yahoo só tem candles diários que atualizam uma vez por dia

2. **Precisa de janela histórica para gerar sinais** - A estratégia SMA precisa ver mudanças de preço ao longo do tempo para detectar cruzamentos

3. **Modo ao vivo vs Modo backtest**:
   - **Modo ao vivo**: Espera por novos dados (lento com candles diários)
   - **Modo backtest**: Reproduz dados históricos (rápido, mostra todas as operações)

## Como Ver Atividade de Trading

### Opção 1: Executar Backtest (Recomendado)
```bash
./build/backtest_yahoo
```
Isso reproduz dados de 2024 dia a dia e mostra todas as operações.

### Opção 2: Executar Backtest Multi-Ativos
```bash
./build/multi_backtest
```
Isso testa 5 ativos diferentes e mostra quais geraram operações.

### Opção 3: Modo ao Vivo (Lento)
Você precisaria:
- Executar o bot continuamente por dias/semanas
- Esperar por sinais reais de cruzamento ocorrerem
- Usar `--source=binance` para atualizações mais rápidas de 1 minuto

## Entendendo os Resultados

### O Que o Backtest Mostra

**Exemplo do backtest SPY:**
- Começou com: $10.000
- Fez 6 operações em 211 dias
- Terminou com: $10.017,75
- Lucro: $17,75 (+0,18%)
- Buy & Hold teria feito: $1.776,01 (+17,76%)

**Por que a estratégia teve desempenho inferior:**
- Cruzamento SMA simples é **muito lento** em mercados em tendência
- Funciona melhor em mercados **laterais/agitados**
- Muitas **operações whipsaw** (comprar alto, vender baixo durante correções)

### Trading Mais Ativo

Dos resultados, **TSLA** gerou mais operações (17) devido à alta volatilidade:
```
Operação #1: COMPRA @ $199.95
Operação #2: VENDA @ $176.54   <- Prejuízo
Operação #3: COMPRA @ $175.22
Operação #4: VENDA @ $171.11   <- Prejuízo
...
Operação #16: VENDA @ $241.05  <- Lucro
Operação #17: COMPRA @ $262.51
```

## Resumo dos Executáveis

| Executável | Propósito | O que mostra |
|------------|-----------|--------------|
| `simple_trading_bot` | Modo trading ao vivo | Espera por sinais em tempo real (lento com dados diários) |
| `backtest_yahoo` | Backtest detalhado único | Log completo de operações para SPY com SMA(5,20) |
| `multi_backtest` | Backtest multi-ativos | Resultados rápidos para 5 ativos diferentes |
| `yahoo_demo` | Teste de API | Mostra capacidades de busca de dados |

## Próximos Passos para Melhorar Trading

1. **Ajustar Parâmetros de Estratégia**
   - Tentar diferentes períodos de SMA (ex: SMA(10, 50))
   - Adicionar filtros de momentum
   - Usar RSI ou MACD em vez disso

2. **Melhor Gerenciamento de Risco**
   - Adicionar ordens stop-loss
   - Dimensionamento de posição baseado em volatilidade
   - Não operar durante períodos de baixo volume

3. **Testar Diferentes Mercados**
   - Encontrar ativos com comportamento de reversão à média
   - Evitar mercados fortemente em tendência para cruzamento SMA

4. **Adicionar Mais Estratégias**
   - Reversão à média
   - Estratégias de breakout
   - Pair trading

## Arquivos Criados para Trading

- `src/backtest_yahoo.cpp` - Backtest detalhado de ativo único
- `src/multi_backtest.cpp` - Backtest de comparação multi-ativos
- `src/YahooFinanceAPI.cpp` - Conector de dados Yahoo Finance
- `src/SMAStrategy.cpp` - Estratégia de cruzamento de média móvel
- `src/paperengine.cpp` - Execução simulada de operações

Todos os componentes estão **conectados e funcionando**. Os backtests provam que todo o pipeline funciona de ponta a ponta! 🎉
