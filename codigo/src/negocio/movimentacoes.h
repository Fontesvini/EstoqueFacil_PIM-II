/* Camada de regras de negócio: entradas e saídas com data, hora e usuário (RF03). */
#ifndef MOVIMENTACOES_H
#define MOVIMENTACOES_H

#include "comum/resultado.h"

/* entrada = 1 para entrada; 0 para saída. */
Resultado podeMovimentar(int entrada);
/* Confere se o produto existe e está ativo. */
Resultado produtoMovimentavel(int posicao);
/* Confere quantidade, saldo suficiente e limite de um inteiro. */
Resultado validarMovimentacao(int posicao, int entrada, int quantidade);
/* Atualiza o saldo e grava o histórico no mesmo arquivo. */
Resultado registrarMovimentacao(int posicao, int entrada, int quantidade);

#endif
