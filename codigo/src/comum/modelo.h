/* Modelo: structs e constantes compartilhadas por todas as camadas.
   Não contém funções. O formato das structs define o arquivo .dat:
   alterar um campo torna incompatíveis os arquivos já gravados. */
#ifndef MODELO_H
#define MODELO_H

#define MAX_USUARIOS 50
#define MAX_PRODUTOS 1000
#define MAX_MOVIMENTACOES 10000
#define TAM_SENHA 65
#define TAM_HASH 9
#define VERSAO_DADOS "EFLOCAL1"

/* Níveis de acesso (RF01). */
enum { NIVEL_ADMIN = 1, NIVEL_GERENTE = 2, NIVEL_OPERADOR = 3, NIVEL_CONSULTA = 4 };

/* Ações do sistema. Os números são os mesmos do menu principal. */
typedef enum {
    ACAO_CONSULTAR = 1, ACAO_ALERTA, ACAO_ENTRADA, ACAO_SAIDA,
    ACAO_CADASTRAR_PRODUTO, ACAO_ALTERAR_PRODUTO, ACAO_SITUACAO_PRODUTO,
    ACAO_RELATORIO, ACAO_BACKUP, ACAO_USUARIOS, ACAO_PROPRIA_SENHA
} Acao;
#define TOTAL_ACOES 11

/* Os registros ficam em vetores. A posição funciona como identificador. */
typedef struct {
    char nome[60], login[30], senhaHash[TAM_HASH];
    int nivel, ativo, erros;
    long long bloqueadoAte;
} Usuario;

typedef struct {
    char codigo[20], nome[60], categoria[40], fornecedor[60];
    int quantidade, minimo, ativo;
} Produto;

typedef struct {
    int produto, usuario, quantidade;
    char tipo; /* E: entrada; S: saida. */
    char data[20];
} Movimentacao;

typedef struct {
    char versao[8];
    int totalUsuarios, totalProdutos, totalMovimentacoes;
    Usuario usuarios[MAX_USUARIOS];
    Produto produtos[MAX_PRODUTOS];
    Movimentacao movimentacoes[MAX_MOVIMENTACOES];
} Dados;

#endif
