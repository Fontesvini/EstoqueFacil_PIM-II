#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "seguranca/contas.h"
#include "seguranca/acesso.h"
#include "comum/validacao.h"
#include "dados/repositorio.h"

void calcularHash(const char *senha, char hash[TAM_HASH]) {
    /* Hash didático DJB2: multiplica por 33 e soma cada caractere.
       NÃO é um algoritmo seguro para senhas reais. Não possui salt.
       uint32_t mantém o mesmo resultado no macOS e no Windows. */
    uint32_t valor = 5381u;
    for (size_t i = 0; senha[i] != '\0'; i++) valor = valor * 33u + (unsigned char)senha[i];
    snprintf(hash, TAM_HASH, "%08lx", (unsigned long)valor);
}

Resultado validarSenha(const char *senha) {
    size_t tamanho = strlen(senha);
    if (tamanho < 8) return ERRO_SENHA_CURTA;
    if (tamanho > TAM_SENHA - 1) return ERRO_SENHA_CARACTERES;
    for (size_t i = 0; i < tamanho; i++)
        if ((unsigned char)senha[i] < 32 || (unsigned char)senha[i] > 126) return ERRO_SENHA_CARACTERES;
    return OK;
}

int quantidadeUsuarios(void) { return dados.totalUsuarios; }

const Usuario *usuarioNaPosicao(int posicao) {
    if (posicao < 0 || posicao >= dados.totalUsuarios) return NULL;
    return &dados.usuarios[posicao];
}

int buscarUsuario(const char *login) {
    for (int i = 0; i < dados.totalUsuarios; i++)
        if (strcmp(dados.usuarios[i].login, login) == 0) return i;
    return -1;
}

Resultado podeCadastrarUsuario(void) {
    /* Sem contas, o primeiro cadastro cria o Administrador inicial. */
    if (dados.totalUsuarios > 0 && !autorizar(ACAO_USUARIOS)) return ERRO_ACESSO_NEGADO;
    if (dados.totalUsuarios == MAX_USUARIOS) return ERRO_LIMITE_USUARIOS;
    return OK;
}

Resultado loginDisponivel(const char *login) {
    char normalizado[sizeof dados.usuarios[0].login];
    if (strlen(login) >= sizeof normalizado) return ERRO_IDENTIFICADOR;
    strcpy(normalizado, login);
    if (!normalizarIdentificador(normalizado, 0) || strcmp(normalizado, login) != 0) return ERRO_IDENTIFICADOR;
    return buscarUsuario(login) >= 0 ? ERRO_LOGIN_EXISTENTE : OK;
}

Resultado cadastrarUsuario(const char *nome, const char *login, int nivel, const char *senha) {
    Usuario novo = {0};
    Resultado r;
    if ((r = podeCadastrarUsuario()) != OK || (r = loginDisponivel(login)) != OK ||
        (r = validarSenha(senha)) != OK) return r;
    if (dados.totalUsuarios == 0) nivel = NIVEL_ADMIN;
    if (nivel < NIVEL_ADMIN || nivel > NIVEL_CONSULTA || nome[0] == '\0' || strlen(nome) >= sizeof novo.nome)
        return ERRO_OPCAO;
    snprintf(novo.nome, sizeof novo.nome, "%s", nome);
    snprintf(novo.login, sizeof novo.login, "%s", login);
    calcularHash(senha, novo.senhaHash);
    novo.nivel = nivel;
    novo.ativo = 1;
    iniciarAlteracao();
    dados.usuarios[dados.totalUsuarios++] = novo;
    return confirmarAlteracao() ? OK : ERRO_GRAVACAO;
}

Resultado podeAlterarConta(int posicao) {
    if (!autorizar(ACAO_USUARIOS)) return ERRO_ACESSO_NEGADO;
    if (usuarioNaPosicao(posicao) == NULL) return ERRO_USUARIO_INEXISTENTE;
    if (posicao == usuarioLogado()) return ERRO_PROPRIA_CONTA;
    return OK;
}

Resultado alterarNivel(int posicao, int nivel) {
    Resultado r = podeAlterarConta(posicao);
    if (r != OK) return r;
    if (nivel < NIVEL_ADMIN || nivel > NIVEL_CONSULTA) return ERRO_OPCAO;
    iniciarAlteracao();
    dados.usuarios[posicao].nivel = nivel;
    return confirmarAlteracao() ? OK : ERRO_GRAVACAO;
}

Resultado alternarSituacaoUsuario(int posicao) {
    Resultado r = podeAlterarConta(posicao);
    if (r != OK) return r;
    iniciarAlteracao();
    dados.usuarios[posicao].ativo = !dados.usuarios[posicao].ativo;
    return confirmarAlteracao() ? OK : ERRO_GRAVACAO;
}

Resultado conferirSenhaAtual(const char *senha) {
    char hash[TAM_HASH];
    if (!autorizar(ACAO_PROPRIA_SENHA)) return ERRO_ACESSO_NEGADO;
    calcularHash(senha, hash);
    return strcmp(hash, dados.usuarios[usuarioLogado()].senhaHash) == 0 ? OK : ERRO_SENHA_ATUAL;
}

Resultado alterarPropriaSenha(const char *atual, const char *nova) {
    Resultado r;
    if ((r = conferirSenhaAtual(atual)) != OK || (r = validarSenha(nova)) != OK) return r;
    iniciarAlteracao();
    calcularHash(nova, dados.usuarios[usuarioLogado()].senhaHash);
    return confirmarAlteracao() ? OK : ERRO_GRAVACAO;
}
