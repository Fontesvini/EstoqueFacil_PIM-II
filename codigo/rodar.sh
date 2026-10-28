#!/bin/bash
# Compila e abre o EstoqueFácil na pasta deste script.
set -eu
PASTA_ESTOQUE="$(cd "$(dirname "$0")" && pwd)"
cd "$PASTA_ESTOQUE"
if ! command -v clang >/dev/null 2>&1; then
    printf '%s\n' 'Compilador clang não encontrado.' 'No Mac, execute: xcode-select --install' 'Depois da instalação, abra o terminal novamente e repita este comando.'
    exit 1
fi
if [ "$(uname -s)" = Darwin ] && ! xcrun --find clang >/dev/null 2>&1; then
    printf '%s\n' 'As ferramentas de desenvolvimento do Mac não estão disponíveis.' 'Execute: xcode-select --install'
    exit 1
fi
# Cada camada fica em uma pasta de src/. -Isrc permite incluir "camada/arquivo.h".
clang -std=c11 -Wall -Wextra -Wpedantic -Werror -Isrc src/main.c src/*/*.c -o estoquefacil
exec ./estoquefacil "$@"
