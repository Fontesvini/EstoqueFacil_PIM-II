#!/bin/bash
# Dois cliques neste arquivo no Finder abrem o EstoqueFácil no Terminal.
# Ele compila o código, se precisar, e abre o programa.
cd "$(dirname "$0")/codigo" || exit 1
bash rodar.sh "$@"
echo
read -r -p "Programa encerrado. Pressione Enter para fechar esta janela..." _
