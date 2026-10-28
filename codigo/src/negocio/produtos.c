#include <string.h>
#include "negocio/produtos.h"
#include "seguranca/acesso.h"
#include "comum/validacao.h"
#include "dados/repositorio.h"

int quantidadeProdutos(void) { return dados.totalProdutos; }

const Produto *produtoNaPosicao(int posicao) {
    if (posicao < 0 || posicao >= dados.totalProdutos) return NULL;
    return &dados.produtos[posicao];
}

int buscarProduto(const char *codigo) {
    for (int i = 0; i < dados.totalProdutos; i++)
        if (strcmp(dados.produtos[i].codigo, codigo) == 0) return i;
    return -1;
}

int estaComEstoqueBaixo(const Produto *produto) {
    return produto->ativo && produto->quantidade <= produto->minimo;
}

int produtoCorresponde(const Produto *produto, const char *busca) {
    return busca[0] == '\0' || strstr(produto->codigo, busca) || strstr(produto->nome, busca);
}

Resultado podeCadastrarProduto(void) {
    if (!autorizar(ACAO_CADASTRAR_PRODUTO)) return ERRO_ACESSO_NEGADO;
    if (dados.totalProdutos == MAX_PRODUTOS) return ERRO_LIMITE_PRODUTOS;
    return OK;
}

Resultado codigoDisponivel(const char *codigo) {
    return buscarProduto(codigo) >= 0 ? ERRO_CODIGO_EXISTENTE : OK;
}

/* Campos de texto precisam terminar dentro do vetor e não podem ficar vazios. */
static int camposValidos(const Produto *p) {
    return p->nome[0] && p->categoria[0] && p->fornecedor[0] && p->minimo >= 0 &&
        memchr(p->nome, 0, sizeof p->nome) && memchr(p->categoria, 0, sizeof p->categoria) &&
        memchr(p->fornecedor, 0, sizeof p->fornecedor);
}

Resultado salvarProduto(int posicao, const Produto *produto) {
    Produto novo = *produto;
    Resultado r;
    if (posicao < 0) {
        char codigo[sizeof novo.codigo];
        if ((r = podeCadastrarProduto()) != OK) return r;
        if (!memchr(novo.codigo, 0, sizeof novo.codigo)) return ERRO_IDENTIFICADOR;
        strcpy(codigo, novo.codigo);
        if (!normalizarIdentificador(codigo, 1) || strcmp(codigo, novo.codigo) != 0) return ERRO_IDENTIFICADOR;
        if ((r = codigoDisponivel(novo.codigo)) != OK) return r;
        novo.quantidade = 0;
        novo.ativo = 1;
    } else {
        if (!autorizar(ACAO_ALTERAR_PRODUTO)) return ERRO_ACESSO_NEGADO;
        if (produtoNaPosicao(posicao) == NULL) return ERRO_PRODUTO_INEXISTENTE;
        /* Código, saldo e situação não mudam nesta operação. */
        memcpy(novo.codigo, dados.produtos[posicao].codigo, sizeof novo.codigo);
        novo.quantidade = dados.produtos[posicao].quantidade;
        novo.ativo = dados.produtos[posicao].ativo;
    }
    if (!camposValidos(&novo)) return ERRO_OPCAO;
    iniciarAlteracao();
    if (posicao < 0) dados.produtos[dados.totalProdutos++] = novo;
    else dados.produtos[posicao] = novo;
    return confirmarAlteracao() ? OK : ERRO_GRAVACAO;
}

Resultado alternarSituacaoProduto(int posicao) {
    if (!autorizar(ACAO_SITUACAO_PRODUTO)) return ERRO_ACESSO_NEGADO;
    if (produtoNaPosicao(posicao) == NULL) return ERRO_PRODUTO_INEXISTENTE;
    iniciarAlteracao();
    dados.produtos[posicao].ativo = !dados.produtos[posicao].ativo;
    return confirmarAlteracao() ? OK : ERRO_GRAVACAO;
}
