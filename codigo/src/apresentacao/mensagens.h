/* Camada de apresentação: transforma o Resultado das outras camadas em texto. */
#ifndef MENSAGENS_H
#define MENSAGENS_H

#include "comum/modelo.h"
#include "comum/resultado.h"

const char *textoDoResultado(Resultado resultado);
/* Mostra a mensagem e devolve 1 somente quando o resultado é OK. */
int mostrarResultado(Resultado resultado);
/* Confere a permissão antes de pedir dados e avisa quando ela falta. */
int permitir(Acao acao);

#endif
