/* Camada de autenticação e controle de acesso: contas de usuário e senhas (RF01).
   Usa somente a camada de dados. Não escreve na tela. */
#ifndef CONTAS_H
#define CONTAS_H

#include "comum/modelo.h"
#include "comum/resultado.h"

/* Hash didático DJB2. NÃO é um algoritmo seguro para senhas reais. */
void calcularHash(const char *senha, char hash[TAM_HASH]);
/* Regras da senha: 8 a 64 caracteres ASCII visíveis ou espaço. */
Resultado validarSenha(const char *senha);

int quantidadeUsuarios(void);
const Usuario *usuarioNaPosicao(int posicao);
int buscarUsuario(const char *login);

/* Confere limite e login antes de pedir os demais campos. */
Resultado podeCadastrarUsuario(void);
Resultado loginDisponivel(const char *login);
/* O primeiro usuário é sempre Administrador e não precisa de sessão. */
Resultado cadastrarUsuario(const char *nome, const char *login, int nivel, const char *senha);

/* A própria conta permanece ativa e Administrador: não se perde o acesso. */
Resultado podeAlterarConta(int posicao);
Resultado alterarNivel(int posicao, int nivel);
Resultado alternarSituacaoUsuario(int posicao);

/* Confere a senha do usuário da sessão. */
Resultado conferirSenhaAtual(const char *senha);
/* Troca a senha do usuário da sessão depois de conferir a atual. */
Resultado alterarPropriaSenha(const char *atual, const char *nova);

#endif
