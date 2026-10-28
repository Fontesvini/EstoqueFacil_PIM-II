#include <stdio.h>
#include <limits.h>
#include "apresentacao/telas.h"
#include "apresentacao/entrada.h"
#include "apresentacao/mensagens.h"
#include "negocio/produtos.h"
#include "negocio/movimentacoes.h"
#include "comum/validacao.h"

/* Lê um código e devolve a posição do produto, ou -1 com aviso. */
static int escolherProduto(void) {
    char codigo[20];
    int posicao;
    if (!lerTexto("Codigo do produto: ", codigo, sizeof codigo)) return -1;
    if (!normalizarIdentificador(codigo, 1)) { mostrarResultado(ERRO_IDENTIFICADOR); return -1; }
    posicao = buscarProduto(codigo);
    if (posicao < 0) mostrarResultado(ERRO_PRODUTO_INEXISTENTE);
    return posicao;
}

void telaAlertaEstoqueBaixo(void) {
    int alertas = 0;
    if (!permitir(ACAO_ALERTA)) return;
    for (int i = 0; i < quantidadeProdutos(); i++) {
        const Produto *p = produtoNaPosicao(i);
        if (estaComEstoqueBaixo(p)) {
            printf("[ESTOQUE BAIXO] %s | %s | quantidade %d | minimo %d\n", p->codigo, p->nome, p->quantidade, p->minimo);
            alertas++;
        }
    }
    if (alertas == 0) puts("Nenhum produto ativo esta com estoque baixo.");
}

void telaConsultarEstoque(void) {
    char busca[60];
    int encontrados = 0;
    if (!permitir(ACAO_CONSULTAR) || !lerLinha("Codigo ou parte do nome (Enter para todos): ", busca, sizeof busca)) return;
    for (int i = 0; i < quantidadeProdutos(); i++) {
        const Produto *p = produtoNaPosicao(i);
        if (produtoCorresponde(p, busca)) {
            printf("%s | %s | saldo %d | minimo %d | %s\n", p->codigo, p->nome,
                p->quantidade, p->minimo, p->ativo ? "ativo" : "inativo");
            encontrados++;
        }
    }
    if (!encontrados) mostrarResultado(ERRO_PRODUTO_INEXISTENTE);
}

void telaManterProduto(int alterar) {
    Produto novo = {0};
    int posicao = -1;
    if (!permitir(alterar ? ACAO_ALTERAR_PRODUTO : ACAO_CADASTRAR_PRODUTO)) return;
    if (alterar) {
        posicao = escolherProduto();
        if (posicao < 0) return;
        novo = *produtoNaPosicao(posicao);
    } else {
        if (!mostrarResultado(podeCadastrarProduto())) return;
        if (!lerTexto("Codigo unico: ", novo.codigo, sizeof novo.codigo)) return;
        if (!normalizarIdentificador(novo.codigo, 1)) { mostrarResultado(ERRO_IDENTIFICADOR); return; }
        if (!mostrarResultado(codigoDisponivel(novo.codigo))) return;
    }
    if (!lerTexto("Nome: ", novo.nome, sizeof novo.nome) ||
        !lerTexto("Categoria: ", novo.categoria, sizeof novo.categoria) ||
        !lerTexto("Fornecedor principal: ", novo.fornecedor, sizeof novo.fornecedor) ||
        !lerInteiro("Estoque minimo (0 a 2147483647): ", 0, INT_MAX, &novo.minimo)) return;
    if (!confirmar("Salvar produto? (s/N): ")) return;
    if (mostrarResultado(salvarProduto(posicao, &novo))) {
        puts("Produto salvo. Use entrada/saida para mudar o saldo.");
        telaAlertaEstoqueBaixo();
    }
}

void telaSituacaoProduto(void) {
    int posicao;
    if (!permitir(ACAO_SITUACAO_PRODUTO)) return;
    posicao = escolherProduto();
    if (posicao < 0 || !confirmar("Inativar/reativar? Saldo e historico serao mantidos. (s/N): ")) return;
    if (mostrarResultado(alternarSituacaoProduto(posicao))) {
        puts("Situacao atualizada.");
        telaAlertaEstoqueBaixo();
    }
}

void telaMovimentacao(int entrada) {
    int posicao, quantidade;
    if (!mostrarResultado(podeMovimentar(entrada))) return;
    posicao = escolherProduto();
    if (posicao < 0 || !mostrarResultado(produtoMovimentavel(posicao))) return;
    printf("Saldo atual: %d\n", produtoNaPosicao(posicao)->quantidade);
    if (!lerInteiro("Quantidade (1 a 2147483647): ", 1, INT_MAX, &quantidade)) return;
    if (!mostrarResultado(validarMovimentacao(posicao, entrada, quantidade))) return;
    if (!confirmar("Confirmar movimentacao? (s/N): ")) return;
    if (mostrarResultado(registrarMovimentacao(posicao, entrada, quantidade))) {
        printf("Movimentacao salva. Novo saldo: %d\n", produtoNaPosicao(posicao)->quantidade);
        telaAlertaEstoqueBaixo();
    }
}
