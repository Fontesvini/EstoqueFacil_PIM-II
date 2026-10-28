/* Camada de autenticação e controle de acesso: login, sessão e permissões (RF01, RNF03). */
#ifndef ACESSO_H
#define ACESSO_H

#include "comum/modelo.h"
#include "comum/resultado.h"

/* Tabela de permissões por nível. Não depende da sessão. */
int verificarPermissao(int nivel, Acao acao);

/* Confere login e senha. Três erros bloqueiam a conta por cinco minutos.
   Em caso de sucesso, a conta passa a ser o usuário da sessão. */
Resultado autenticar(const char *login, const char *senha);

/* Posição do usuário da sessão, ou -1 antes do login. */
int usuarioLogado(void);

/* Confere se o usuário da sessão está ativo e tem permissão para a ação. */
int autorizar(Acao acao);

#endif
