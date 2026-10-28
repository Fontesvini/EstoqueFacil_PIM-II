#include <string.h>
#include <time.h>
#include "seguranca/acesso.h"
#include "seguranca/contas.h"
#include "dados/repositorio.h"

#define TENTATIVAS_BLOQUEIO 3
#define SEGUNDOS_BLOQUEIO 300

static int usuarioAtual = -1;

/* Os números das ações são os mesmos do menu. */
int verificarPermissao(int nivel, Acao acao) {
    if (nivel < NIVEL_ADMIN || nivel > NIVEL_CONSULTA) return 0;
    switch (acao) {
        case ACAO_CONSULTAR: case ACAO_ALERTA: case ACAO_PROPRIA_SENHA: return 1;
        case ACAO_ENTRADA: case ACAO_SAIDA: return nivel <= NIVEL_OPERADOR;
        case ACAO_CADASTRAR_PRODUTO: case ACAO_ALTERAR_PRODUTO:
        case ACAO_SITUACAO_PRODUTO: case ACAO_BACKUP: return nivel <= NIVEL_GERENTE;
        case ACAO_RELATORIO: return nivel != NIVEL_OPERADOR;
        case ACAO_USUARIOS: return nivel == NIVEL_ADMIN;
        default: return 0;
    }
}

int usuarioLogado(void) { return usuarioAtual; }

int autorizar(Acao acao) {
    return usuarioAtual >= 0 && dados.usuarios[usuarioAtual].ativo &&
        verificarPermissao(dados.usuarios[usuarioAtual].nivel, acao);
}

Resultado autenticar(const char *login, const char *senha) {
    char hash[TAM_HASH];
    int posicao = buscarUsuario(login), correto;
    long long agora;
    Usuario *u;
    if (posicao < 0) return ERRO_LOGIN_INCORRETO;
    u = &dados.usuarios[posicao];
    if (!u->ativo) return ERRO_USUARIO_INATIVO;
    agora = (long long)time(NULL);
    if (u->bloqueadoAte > agora) return ERRO_USUARIO_BLOQUEADO;
    iniciarAlteracao();
    /* Bloqueio vencido: o contador recomeça. */
    if (u->bloqueadoAte != 0) { u->erros = 0; u->bloqueadoAte = 0; }
    calcularHash(senha, hash);
    correto = strcmp(hash, u->senhaHash) == 0;
    if (correto) u->erros = 0;
    else {
        u->erros++;
        if (u->erros == TENTATIVAS_BLOQUEIO) u->bloqueadoAte = agora + SEGUNDOS_BLOQUEIO;
    }
    /* O contador fica no arquivo: o bloqueio continua após reabrir. */
    if (!confirmarAlteracao()) return ERRO_GRAVACAO;
    if (!correto) return u->erros == TENTATIVAS_BLOQUEIO ? ERRO_BLOQUEIO_INICIADO : ERRO_LOGIN_INCORRETO;
    usuarioAtual = posicao;
    return OK;
}
