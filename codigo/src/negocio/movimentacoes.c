#include <limits.h>
#include <time.h>
#include "negocio/movimentacoes.h"
#include "negocio/produtos.h"
#include "seguranca/acesso.h"
#include "dados/repositorio.h"

Resultado podeMovimentar(int entrada) {
    if (!autorizar(entrada ? ACAO_ENTRADA : ACAO_SAIDA)) return ERRO_ACESSO_NEGADO;
    if (dados.totalMovimentacoes == MAX_MOVIMENTACOES) return ERRO_LIMITE_HISTORICO;
    return OK;
}

Resultado produtoMovimentavel(int posicao) {
    const Produto *p = produtoNaPosicao(posicao);
    if (p == NULL) return ERRO_PRODUTO_INEXISTENTE;
    return p->ativo ? OK : ERRO_PRODUTO_INATIVO;
}

Resultado validarMovimentacao(int posicao, int entrada, int quantidade) {
    const Produto *p;
    Resultado r = produtoMovimentavel(posicao);
    if (r != OK) return r;
    p = produtoNaPosicao(posicao);
    if (quantidade < 1) return ERRO_QUANTIDADE;
    if (!entrada && quantidade > p->quantidade) return ERRO_ESTOQUE_INSUFICIENTE;
    if (entrada && quantidade > INT_MAX - p->quantidade) return ERRO_ESTOURO;
    return OK;
}

Resultado registrarMovimentacao(int posicao, int entrada, int quantidade) {
    Movimentacao nova = {0};
    Produto *p;
    time_t agora;
    struct tm *horario;
    Resultado r;
    if ((r = podeMovimentar(entrada)) != OK || (r = validarMovimentacao(posicao, entrada, quantidade)) != OK) return r;
    agora = time(NULL); horario = localtime(&agora);
    if (horario == NULL || !strftime(nova.data, sizeof nova.data, "%Y-%m-%d %H:%M:%S", horario)) return ERRO_DATA_HORA;
    nova.produto = posicao; nova.usuario = usuarioLogado(); nova.quantidade = quantidade;
    nova.tipo = entrada ? 'E' : 'S';
    p = &dados.produtos[posicao];
    iniciarAlteracao();
    if (entrada) p->quantidade += quantidade;
    else p->quantidade -= quantidade;
    dados.movimentacoes[dados.totalMovimentacoes++] = nova;
    /* Saldo e histórico ficam juntos no mesmo arquivo. */
    return confirmarAlteracao() ? OK : ERRO_GRAVACAO;
}
