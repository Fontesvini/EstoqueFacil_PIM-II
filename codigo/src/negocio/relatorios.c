#include <string.h>
#include "negocio/relatorios.h"
#include "seguranca/acesso.h"
#include "comum/validacao.h"
#include "dados/repositorio.h"

Resultado validarPeriodo(const char *inicio, const char *fim) {
    if ((inicio[0] && !dataValida(inicio)) || (fim[0] && !dataValida(fim)) ||
        (inicio[0] && fim[0] && strcmp(inicio, fim) > 0)) return ERRO_PERIODO;
    return OK;
}

static void escreverEstoque(FILE *arquivo, const char *codigo) {
    for (int i = 0; i < dados.totalProdutos; i++) {
        Produto *p = &dados.produtos[i];
        if (!codigo[0] || strcmp(codigo, p->codigo) == 0)
            fprintf(arquivo, "%s | %s | saldo %d | minimo %d | %s\n", p->codigo, p->nome,
                p->quantidade, p->minimo, p->ativo ? "ativo" : "inativo");
    }
}

static void escreverHistorico(FILE *arquivo, const char *codigo, const char *inicio, const char *fim) {
    for (int i = 0; i < dados.totalMovimentacoes; i++) {
        Movimentacao *m = &dados.movimentacoes[i];
        if (codigo[0] && strcmp(codigo, dados.produtos[m->produto].codigo) != 0) continue;
        if (inicio[0] && strncmp(m->data, inicio, 10) < 0) continue;
        if (fim[0] && strncmp(m->data, fim, 10) > 0) continue;
        fprintf(arquivo, "%s | %s | %s | %d | %s\n", m->data, dados.produtos[m->produto].codigo,
            m->tipo == 'E' ? "ENTRADA" : "SAIDA", m->quantidade, dados.usuarios[m->usuario].nome);
    }
}

Resultado gerarRelatorio(int tipo, const char *codigo, const char *inicio, const char *fim) {
    FILE *arquivo;
    Resultado r;
    if (!autorizar(ACAO_RELATORIO)) return ERRO_ACESSO_NEGADO;
    if (tipo != RELATORIO_ESTOQUE && tipo != RELATORIO_HISTORICO) return ERRO_OPCAO;
    if ((r = validarPeriodo(inicio, fim)) != OK) return r;
    arquivo = abrirRelatorio();
    if (arquivo == NULL) return ERRO_RELATORIO_CRIAR;
    /* Senhas e hashes nunca aparecem no relatório. */
    fprintf(arquivo, "ESTOQUEFACIL - %s\n", tipo == RELATORIO_ESTOQUE ? "ESTOQUE" : "HISTORICO");
    if (tipo == RELATORIO_ESTOQUE) escreverEstoque(arquivo, codigo);
    else escreverHistorico(arquivo, codigo, inicio, fim);
    return fecharRelatorio(arquivo) ? OK : ERRO_RELATORIO_GRAVAR;
}

Resultado fazerBackup(void) {
    if (!autorizar(ACAO_BACKUP)) return ERRO_ACESSO_NEGADO;
    return salvarBackup() ? OK : ERRO_BACKUP;
}
