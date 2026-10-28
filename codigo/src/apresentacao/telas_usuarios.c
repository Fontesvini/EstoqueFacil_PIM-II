#include <stdio.h>
#include <string.h>
#include "apresentacao/telas.h"
#include "apresentacao/entrada.h"
#include "apresentacao/mensagens.h"
#include "seguranca/acesso.h"
#include "seguranca/contas.h"
#include "comum/validacao.h"

/* Pede a senha duas vezes e confere as regras antes de continuar. */
static int lerNovaSenha(char *senha) {
    char repetida[TAM_SENHA];
    if (!lerSenha("Nova senha (8 a 64 caracteres ASCII): ", senha, TAM_SENHA)) return 0;
    if (!mostrarResultado(validarSenha(senha))) return 0;
    if (!lerSenha("Repita a nova senha: ", repetida, sizeof repetida)) return 0;
    if (strcmp(senha, repetida) != 0) { puts("As senhas nao conferem."); return 0; }
    return 1;
}

void telaCadastrarUsuario(void) {
    char nome[60], login[30], senha[TAM_SENHA];
    int nivel = NIVEL_ADMIN, primeiro = quantidadeUsuarios() == 0;
    if (!mostrarResultado(podeCadastrarUsuario())) return;
    if (!lerTexto("Nome: ", nome, sizeof nome) || !lerTexto("Login: ", login, sizeof login)) return;
    if (!normalizarIdentificador(login, 0)) { mostrarResultado(ERRO_IDENTIFICADOR); return; }
    if (!mostrarResultado(loginDisponivel(login))) return;
    if (!primeiro && !lerInteiro("Nivel (1 Admin, 2 Gerente, 3 Operador, 4 Consulta): ", 1, 4, &nivel)) return;
    if (!lerNovaSenha(senha)) return;
    if (mostrarResultado(cadastrarUsuario(nome, login, nivel, senha))) puts("Usuario cadastrado.");
}

int telaLogin(void) {
    char login[30], senha[TAM_SENHA];
    for (int tentativa = 1; tentativa <= 3; tentativa++) {
        Resultado r;
        printf("\nTentativa %d de 3. Enter no login encerra.\n", tentativa);
        if (!lerLinha("Login: ", login, sizeof login) || login[0] == '\0') return 0;
        if (!normalizarIdentificador(login, 0)) { mostrarResultado(ERRO_IDENTIFICADOR); continue; }
        if (!lerSenha("Senha: ", senha, sizeof senha)) return 0;
        r = autenticar(login, senha);
        if (r == OK) return 1;
        mostrarResultado(r);
        if (r == ERRO_GRAVACAO) return 0;
    }
    return 0;
}

void telaAdministrarUsuarios(void) {
    int opcao, id, nivel;
    if (!permitir(ACAO_USUARIOS)) return;
    for (int i = 0; i < quantidadeUsuarios(); i++) {
        const Usuario *u = usuarioNaPosicao(i);
        printf("%d | %s | nivel %d | %s\n", i + 1, u->nome, u->nivel, u->ativo ? "ativo" : "inativo");
    }
    if (!lerInteiro("1 Cadastrar, 2 Alterar nivel, 3 Inativar/reativar, 0 Voltar: ", 0, 3, &opcao) || opcao == 0) return;
    if (opcao == 1) { telaCadastrarUsuario(); return; }
    if (!lerInteiro("Numero do usuario: ", 1, quantidadeUsuarios(), &id)) return;
    id--;
    if (!mostrarResultado(podeAlterarConta(id))) return;
    if (opcao == 2) {
        if (!lerInteiro("Novo nivel (1 a 4): ", 1, 4, &nivel)) return;
        if (mostrarResultado(alterarNivel(id, nivel))) puts("Usuario atualizado.");
    } else {
        if (!confirmar("Confirma inativar/reativar esta conta? (s/N): ")) return;
        if (mostrarResultado(alternarSituacaoUsuario(id))) puts("Usuario atualizado.");
    }
}

void telaAlterarSenha(void) {
    char atual[TAM_SENHA], nova[TAM_SENHA];
    if (!permitir(ACAO_PROPRIA_SENHA) || !lerSenha("Senha atual: ", atual, sizeof atual)) return;
    /* Confere a senha atual antes de pedir a nova. */
    if (!mostrarResultado(conferirSenhaAtual(atual))) return;
    if (!lerNovaSenha(nova)) return;
    if (mostrarResultado(alterarPropriaSenha(atual, nova))) puts("Senha alterada.");
}
