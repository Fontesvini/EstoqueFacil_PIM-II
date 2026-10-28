#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#else
#include <unistd.h>
#endif
#include "dados/repositorio.h"

Dados dados;
static Dados antes;
static char pasta[4096] = ".";

int usarPastaDoPrograma(const char *programa) {
    char caminho[4096], *barra;
#ifdef _WIN32
    char *outra;
    if (_fullpath(caminho, programa, sizeof caminho) == NULL) return 0;
    barra = strrchr(caminho, '\\');
    outra = strrchr(caminho, '/');
    if (outra > barra) barra = outra;
    if (barra == NULL) return 0;
    *barra = '\0';
    if (_chdir(caminho) != 0) return 0;
#else
    /* Sem barra, o programa veio do PATH: fica a pasta atual. */
    if (strchr(programa, '/') == NULL || realpath(programa, caminho) == NULL) return 0;
    barra = strrchr(caminho, '/');
    if (barra == caminho) barra[1] = '\0';
    else *barra = '\0';
    if (chdir(caminho) != 0) return 0;
#endif
    snprintf(pasta, sizeof pasta, "%s", caminho);
    return 1;
}

const char *pastaDosDados(void) { return pasta; }

static int salvarArquivo(const char *nome) {
    char temporario[80];
    int ok;
    FILE *arquivo;
    snprintf(temporario, sizeof temporario, "%s.tmp", nome);
    arquivo = fopen(temporario, "wb");
    if (arquivo == NULL) return 0;
    ok = fwrite(&dados, sizeof dados, 1, arquivo) == 1;
    if (fclose(arquivo) != 0) ok = 0;
    /* Só substitui o arquivo anterior depois de terminar a escrita. */
    if (ok) {
#ifdef _WIN32
        ok = MoveFileExA(temporario, nome, MOVEFILE_REPLACE_EXISTING) != 0;
#else
        ok = rename(temporario, nome) == 0;
#endif
    }
    if (!ok) remove(temporario);
    return ok;
}

void iniciarAlteracao(void) { antes = dados; }

void desfazerAlteracao(void) { dados = antes; }

int confirmarAlteracao(void) {
    if (salvarArquivo(ARQUIVO_DADOS)) return 1;
    desfazerAlteracao();
    return 0;
}

int salvarBackup(void) { return salvarArquivo(ARQUIVO_BACKUP); }

FILE *abrirRelatorio(void) { return fopen(ARQUIVO_RELATORIO, "w"); }

int fecharRelatorio(FILE *arquivo) {
    int ok = !ferror(arquivo);
    if (fclose(arquivo) != 0) ok = 0;
    return ok;
}

int carregarDados(void) {
    FILE *arquivo = fopen(ARQUIVO_DADOS, "rb");
    int ok;
    if (arquivo == NULL) {
        if (errno != ENOENT) return 0;
        memcpy(dados.versao, VERSAO_DADOS, 8);
        return 1;
    }
    ok = fread(&dados, sizeof dados, 1, arquivo) == 1;
    if (fgetc(arquivo) != EOF || ferror(arquivo)) ok = 0;
    fclose(arquivo);
    if (!ok || memcmp(dados.versao, VERSAO_DADOS, 8) != 0) return 0;
    if (dados.totalUsuarios < 1 || dados.totalUsuarios > MAX_USUARIOS ||
        dados.totalProdutos < 0 || dados.totalProdutos > MAX_PRODUTOS ||
        dados.totalMovimentacoes < 0 || dados.totalMovimentacoes > MAX_MOVIMENTACOES) return 0;
    /* Confere os limites antes de usar um arquivo que veio do disco. */
    for (int i = 0; i < dados.totalUsuarios; i++) {
        Usuario *u = &dados.usuarios[i];
        if (u->nivel < 1 || u->nivel > 4 || u->ativo < 0 || u->ativo > 1 || u->erros < 0 || u->erros > 3 ||
            !memchr(u->nome, 0, sizeof u->nome) || !memchr(u->login, 0, sizeof u->login) ||
            !memchr(u->senhaHash, 0, sizeof u->senhaHash)) return 0;
    }
    for (int i = 0; i < dados.totalProdutos; i++) {
        Produto *p = &dados.produtos[i];
        if (p->quantidade < 0 || p->minimo < 0 || p->ativo < 0 || p->ativo > 1 ||
            !memchr(p->codigo, 0, sizeof p->codigo) || !memchr(p->nome, 0, sizeof p->nome) ||
            !memchr(p->categoria, 0, sizeof p->categoria) || !memchr(p->fornecedor, 0, sizeof p->fornecedor)) return 0;
    }
    for (int i = 0; i < dados.totalMovimentacoes; i++) {
        Movimentacao *m = &dados.movimentacoes[i];
        if (m->produto < 0 || m->produto >= dados.totalProdutos || m->usuario < 0 || m->usuario >= dados.totalUsuarios ||
            m->quantidade < 1 || (m->tipo != 'E' && m->tipo != 'S') || !memchr(m->data, 0, sizeof m->data)) return 0;
    }
    return 1;
}
