#!/bin/bash
# Gera os nove diagramas sem alterar o código ou os dados do EstoqueFácil.
set -euo pipefail

PASTA_DIAGRAMAS="$(cd "$(dirname "$0")" && pwd)"
ALTERNATIVOS=0
case "${1:-}" in
    "") ;;
    --alternativos) ALTERNATIVOS=1 ;;
    --ajuda|-h|--help)
        printf '%s\n' 'Uso: bash gerar_diagramas.sh [--alternativos]' \
            'Sem opção: gera os nove PNGs e SVGs dos fontes DOT.' \
            '--alternativos: também gera as versões PlantUML e Mermaid em imagens/alternativos/.'
        exit 0 ;;
    *) printf '%s\n' 'Opção desconhecida. Use --ajuda.' >&2; exit 2 ;;
esac

# Apenas informa comandos de instalação. Nenhum pacote é instalado pelo script.
FALTA_DOT=0
FALTA_ALTERNATIVO=0
if ! command -v dot >/dev/null 2>&1; then
    printf '%s\n' 'Graphviz não encontrado. No Mac com Homebrew: brew install graphviz'
    FALTA_DOT=1
fi
if ! command -v plantuml >/dev/null 2>&1; then
    printf '%s\n' 'PlantUML não encontrado. Opcional no modo padrão. No Mac: brew install plantuml'
    FALTA_ALTERNATIVO=1
fi
if ! command -v mmdc >/dev/null 2>&1; then
    printf '%s\n' 'Mermaid CLI não encontrado. Opcional no modo padrão.' \
        'Com Node.js e npm: npm install -g @mermaid-js/mermaid-cli'
    if ! command -v npm >/dev/null 2>&1; then
        printf '%s\n' 'Node.js e npm no Mac com Homebrew: brew install node'
    fi
    FALTA_ALTERNATIVO=1
fi
if [ "$FALTA_DOT" -eq 1 ]; then
    printf '%s\n' 'Os PNGs entregues já estão prontos em imagens/. Instale Graphviz somente se quiser regenerá-los.' >&2
    exit 2
fi
if [ "$ALTERNATIVOS" -eq 1 ] && [ "$FALTA_ALTERNATIVO" -eq 1 ]; then
    printf '%s\n' 'Para --alternativos, disponibilize PlantUML e Mermaid CLI, ou execute sem essa opção.' >&2
    exit 2
fi

mkdir -p "$PASTA_DIAGRAMAS/imagens" "$PASTA_DIAGRAMAS/vetores"
TOTAL_DIAGRAMAS=0
for FONTE_DIAGRAMA in "$PASTA_DIAGRAMAS"/fontes/*.dot; do
    [ -f "$FONTE_DIAGRAMA" ] || continue
    NOME_DIAGRAMA="$(basename "$FONTE_DIAGRAMA" .dot)"
    # neato respeita as posições dos elementos definidas nos fontes.
    dot -Kneato -Gdpi=300 -Tpng "$FONTE_DIAGRAMA" -o "$PASTA_DIAGRAMAS/imagens/$NOME_DIAGRAMA.png"
    dot -Kneato -Tsvg "$FONTE_DIAGRAMA" -o "$PASTA_DIAGRAMAS/vetores/$NOME_DIAGRAMA.svg"
    TOTAL_DIAGRAMAS=$((TOTAL_DIAGRAMAS + 1))
    printf 'Gerado: %s.png\n' "$NOME_DIAGRAMA"
done
if [ "$TOTAL_DIAGRAMAS" -ne 9 ]; then
    printf 'Esperados nove fontes DOT; encontrados %s. Confira a pasta fontes/.\n' "$TOTAL_DIAGRAMAS" >&2
    exit 1
fi

if [ "$ALTERNATIVOS" -eq 1 ]; then
    mkdir -p "$PASTA_DIAGRAMAS/imagens/alternativos"
    for FONTE_DIAGRAMA in "$PASTA_DIAGRAMAS"/fontes/*.puml; do
        [ -f "$FONTE_DIAGRAMA" ] || continue
        plantuml -charset UTF-8 -o "$PASTA_DIAGRAMAS/imagens/alternativos" "$FONTE_DIAGRAMA"
    done
    for FONTE_DIAGRAMA in "$PASTA_DIAGRAMAS"/fontes/*.mmd; do
        [ -f "$FONTE_DIAGRAMA" ] || continue
        NOME_DIAGRAMA="$(basename "$FONTE_DIAGRAMA" .mmd)"
        mmdc -i "$FONTE_DIAGRAMA" -o "$PASTA_DIAGRAMAS/imagens/alternativos/$NOME_DIAGRAMA.png" -b white --size 3000
    done
fi
printf '%s\n' 'Concluído. As imagens estão em imagens/.' \
    'Se você alterou algum fonte, substitua também a imagem correspondente nos documentos Word.'
