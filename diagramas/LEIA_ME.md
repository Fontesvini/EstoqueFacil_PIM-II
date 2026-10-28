# Diagramas do EstoqueFácil

As nove imagens estão prontas na pasta `imagens`. Você pode inseri-las no Word sem instalar ferramentas de desenho. O caderno em Word reúne todos os diagramas, cada um com título, fonte e uma explicação curta.

Os desenhos correspondem ao programa simples em C com arquivo local. Não há servidor, SQLite nem comandos SQL nesta versão.

## Arquivos entregues

- `Caderno_de_diagramas_EstoqueFacil.docx`: documento separado com os nove diagramas.
- `imagens/`: nove PNGs com fundo branco e 2.400 pixels de largura, verificados visualmente.
- `fontes/`: nove arquivos DOT usados para gerar os PNGs, seis fontes alternativos PlantUML e um Mermaid.
- `vetores/`: nove SVGs, que podem ser ampliados sem perder definição.
- `gerar_diagramas.sh`: script para regenerar as imagens depois de editar os fontes.
- `dimensoes.json`: dimensões dos PNGs da entrega original, antes de qualquer regeneração local.

## Onde inserir no PIM

A numeração das figuras no texto principal é diferente da ordem dos nove arquivos. As quatro imagens abaixo já estão inseridas no Word principal. Use este quadro se precisar substituí-las:

| Arquivo | Local no PIM principal | Título acima da imagem |
| --- | --- | --- |
| 01_casos_de_uso_geral.png | Etapa 3, Engenharia de Software Ágil | Figura 1 - Casos de uso e permissões do EstoqueFácil |
| 08_der.png | Etapa 4, Modelagem de Banco de Dados | Figura 2 - Registros e relacionamentos do EstoqueFácil |
| 05_fluxograma_login.png | Etapa 5, Programação Estruturada em C | Figura 3 - Fluxo do login com até três rodadas |
| 09_rede.png | Etapa 6, Redes e Sistemas Distribuídos | Figura 4 - Rede proposta para a Papelaria Ponto do Caderno |

Os outros cinco desenhos ficam no caderno separado. Abaixo de cada figura, escreva: `Fonte: elaborado pelo grupo (2026)`.

No Word, escolha Inserir > Imagens e selecione o PNG. Mantenha a proporção. Em uma página A4 com margem esquerda de 3 cm e direita de 2 cm, a largura disponível é 16 cm. Os diagramas mais altos precisam de uma página própria para continuarem legíveis. O caderno já mostra uma forma de posicioná-los. O PIM final já foi paginado com essas quatro imagens reais. Reconfira a paginação se fizer alterações.

## Como ler os desenhos

Consulta e Operador têm permissões diferentes. Por isso, ambos herdam as ações comuns de Usuário. Gerente reúne as permissões dos dois e Administrador herda as de Gerente. O Operador não gera relatório.

Os nomes Usuario, Produto, Movimentacao, Dados e os nomes das funções seguem o código. O diagrama de classes é uma representação didática das structs e dos grupos de funções de C. Os pacotes são as camadas do código: cada um corresponde a uma pasta de `codigo/src`, e uma camada só usa as de baixo.

O modelo conceitual e o DER mostram os dados realmente utilizados: categoria e fornecedor são textos do produto; o nível é um número de 1 a 4 do usuário. Os relacionamentos são mantidos por posições nos vetores e conferidos pelo código. Registros são inativados, sem remover posições.

No fluxograma de login, há até três rodadas por execução da função. O contador de erros pertence a cada conta e fica gravado no arquivo. Três senhas incorretas para uma conta ativa causam bloqueio de 300 segundos. A rede desenhada é uma proposta; o programa atual funciona em um computador e não sincroniza com celulares.

## Regenerar depois de editar

Para usar os PNGs prontos, ignore esta seção.

Abra o terminal na pasta `diagramas` e rode:

```bash
bash gerar_diagramas.sh
```

Esse comando precisa apenas do Graphviz e refaz todos os nove diagramas a partir dos arquivos `.dot`. Ele substitui os PNGs e os SVGs, mas não atualiza as imagens já inseridas nos documentos Word. Dependendo da versão do Graphviz e das fontes instaladas, o resultado pode ter pequenas diferenças visuais e de dimensões.

O script confere Graphviz, PlantUML e Mermaid CLI. Quando falta uma ferramenta, ele mostra o comando correspondente. Ele não instala nada automaticamente. Com Homebrew já disponível no Mac, os comandos são:

```bash
brew install graphviz
brew install plantuml
brew install node
npm install -g @mermaid-js/mermaid-cli
```

PlantUML e Mermaid CLI são opcionais para o modo padrão. Para usar também os fontes `.puml` e `.mmd`, rode:

```bash
bash gerar_diagramas.sh --alternativos
```

As versões alternativas ficam em `imagens/alternativos`, com diagramação automática. Edite o DOT para mudar a imagem principal; se mantiver os formatos alternativos, atualize também os arquivos correspondentes. O script não converte alterações de um formato para outro.

## Verificação realizada

Os nove PNGs e SVGs foram gerados pelo motor Graphviz disponível no ambiente, por meio de Viz.js. Os PNGs foram revisados individualmente e dentro do caderno em Word. O script passou pela verificação de sintaxe do Bash e pelo teste de aviso de ferramentas ausentes. A execução completa do script com os programas nativos Graphviz, PlantUML e Mermaid CLI não foi realizada porque esses comandos não estão instalados neste ambiente. Os PNGs entregues não dependem dessa execução.

## Documentação das ferramentas

- [Linha de comando do Graphviz](https://graphviz.org/doc/info/command.html).
- [Linha de comando do PlantUML](https://plantuml.com/command-line).
- [Instalação e uso do Mermaid CLI](https://github.com/mermaid-js/mermaid-cli).

Consulta em: 11 out. 2026. Essas páginas orientam a geração das imagens. Os diagramas representam o projeto do grupo.
