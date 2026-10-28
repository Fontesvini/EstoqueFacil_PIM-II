/* Camada de acesso a dados: única parte do programa que abre arquivos.
   fread carrega os vetores; fwrite guarda os vetores. */
#ifndef REPOSITORIO_H
#define REPOSITORIO_H

#include <stdio.h>
#include "comum/modelo.h"

#define ARQUIVO_DADOS "estoque_local.dat"
#define ARQUIVO_BACKUP "backup_local.dat"
#define ARQUIVO_RELATORIO "relatorio.txt"

/* Registros carregados na memória. As camadas de cima leem e alteram
   estes vetores entre iniciarAlteracao e confirmarAlteracao. */
extern Dados dados;

/* Usa a pasta src/output/, junto do código, para todos os arquivos (dados,
   backup e relatório) e a cria se não existir. Assim tudo fica no mesmo lugar,
   de qualquer pasta que o programa seja aberto. Devolve 0 se não conseguir. */
int usarPastaDoPrograma(const char *programa);
/* Caminho completo da pasta onde os arquivos são salvos. */
const char *pastaDosDados(void);

/* Lê estoque_local.dat e confere os limites. Sem arquivo, começa vazio. */
int carregarDados(void);

/* Guarda uma cópia da situação atual para desfazer em caso de falha. */
void iniciarAlteracao(void);
/* Grava o arquivo. Se a gravação falhar, desfaz e devolve 0. */
int confirmarAlteracao(void);
/* Volta para a cópia guardada em iniciarAlteracao. */
void desfazerAlteracao(void);

/* Salva backup_local.dat. Em falha, a cópia anterior é mantida. */
int salvarBackup(void);

/* Abre e fecha relatorio.txt. fecharRelatorio devolve 0 se houve erro. */
FILE *abrirRelatorio(void);
int fecharRelatorio(FILE *arquivo);

#endif
