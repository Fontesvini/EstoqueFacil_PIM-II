/* Camada de regras de negócio: produtos e alerta de estoque baixo (RF02, RF04, RF05). */
#ifndef PRODUTOS_H
#define PRODUTOS_H

#include "comum/modelo.h"
#include "comum/resultado.h"

int quantidadeProdutos(void);
const Produto *produtoNaPosicao(int posicao);
int buscarProduto(const char *codigo);

/* RF04: produto ativo com saldo menor ou igual ao mínimo. */
int estaComEstoqueBaixo(const Produto *produto);
/* Busca por código ou parte do nome. Texto vazio corresponde a todos. */
int produtoCorresponde(const Produto *produto, const char *busca);

/* Confere permissão, limite e código antes de pedir os demais campos. */
Resultado podeCadastrarProduto(void);
Resultado codigoDisponivel(const char *codigo);

/* posicao -1 cadastra; outro valor altera. O saldo só muda por movimentação. */
Resultado salvarProduto(int posicao, const Produto *produto);
/* Inativar mantém saldo e histórico. */
Resultado alternarSituacaoProduto(int posicao);

#endif
