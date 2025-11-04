# Como Habilitar o GitHub Pages

Este repositório está configurado para usar GitHub Pages. Siga estes passos para ativar o site:

## Passos para Ativação

1. **Faça commit dos arquivos de configuração:**
   ```bash
   git add _config.yml index.md _layouts/ docs/
   git commit -m "Add GitHub Pages configuration"
   git push origin main
   ```

2. **Habilite GitHub Pages no repositório:**
   - Vá para o repositório no GitHub: https://github.com/vitornegromonte/trading-bot
   - Clique em **Settings** (Configurações)
   - No menu lateral, clique em **Pages**
   - Em **Source** (Fonte), selecione:
     - Branch: `main`
     - Folder: `/ (root)`
   - Clique em **Save** (Salvar)

3. **Aguarde a publicação:**
   - O GitHub levará alguns minutos para publicar o site
   - Quando pronto, uma mensagem aparecerá com a URL do site
   - URL será: `https://vitornegromonte.github.io/trading-bot/`

## Estrutura dos Arquivos

```
trading-bot/
├── _config.yml              # Configuração do Jekyll
├── index.md                 # Página inicial (renderiza README.md)
├── _layouts/
│   └── default.html         # Layout customizado
└── docs/
    ├── config.md            # Página de configuração
    ├── trading.md           # Página de trading
    └── reference.md         # Página de referência
```

## Atualizando o Conteúdo

O site é gerado automaticamente a partir dos arquivos Markdown:

- **Página Principal**: Editando `README.md`, a página inicial será atualizada
- **Configuração**: Editando `CONFIG_GUIDE.md`, a página /docs/config.html será atualizada
- **Trading**: Editando `TRADING_GUIDE.md`, a página /docs/trading.html será atualizada
- **Referência**: Editando `QUICK_REFERENCE.md`, a página /docs/reference.html será atualizada

Após fazer commit das mudanças, o GitHub Pages será automaticamente reconstruído.

## Testando Localmente (Opcional)

Para testar o site localmente antes de publicar:

```bash
# Instalar Jekyll
gem install bundler jekyll

# Criar Gemfile
cat > Gemfile << EOF
source "https://rubygems.org"
gem "github-pages", group: :jekyll_plugins
gem "jekyll-seo-tag"
EOF

# Instalar dependências
bundle install

# Executar servidor local
bundle exec jekyll serve

# Acessar: http://localhost:4000
```

## Tema e Customização

- **Tema**: Cayman (tema oficial do GitHub)
- **Cor Principal**: Azul (#155799)
- **Layout**: Responsivo e mobile-friendly
- **Navegação**: Menu no topo de cada página
- **Features**:
  - Syntax highlighting para código
  - Tabelas estilizadas
  - Links para documentação
  - Botões de download

## Verificar Status

Após habilitar, você pode verificar o status do deploy em:
- Settings → Pages → Verá "Your site is live at..."
- Actions tab → Verá os workflows de "pages build and deployment"

## Troubleshooting

Se o site não aparecer:
1. Verifique se os arquivos foram commitados corretamente
2. Verifique se GitHub Pages está habilitado em Settings → Pages
3. Aguarde alguns minutos após o primeiro deploy
4. Verifique a aba Actions para ver se há erros no build

## Links do Site

Após a publicação, o site terá estas páginas:

- 🏠 **Início**: https://vitornegromonte.github.io/trading-bot/
- ⚙️ **Configuração**: https://vitornegromonte.github.io/trading-bot/docs/config.html
- 📈 **Trading**: https://vitornegromonte.github.io/trading-bot/docs/trading.html
- 📚 **Referência**: https://vitornegromonte.github.io/trading-bot/docs/reference.html
