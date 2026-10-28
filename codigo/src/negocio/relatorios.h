/* Camada de regras de negócio: relatório de texto e backup (RF05, RNF04). Não há comandos SQL. */
#ifndef RELATORIOS_H
#define RELATORIOS_H

#include "comum/resultado.h"

enum { RELATORIO_ESTOQUE = 1, RELATORIO_HISTORICO = 2 };

/* Datas AAAA-MM-DD; texto vazio significa sem limite. */
Resultado validarPeriodo(const char *inicio, const char *fim);
/* Grava relatorio.txt. codigo vazio inclui todos os produtos. */
Resultado gerarRelatorio(int tipo, const char *codigo, const char *inicio, const char *fim);
Resultado fazerBackup(void);

#endif
