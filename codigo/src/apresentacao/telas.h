/* Camada de apresentação: telas. Cada tela pede os dados, chama uma regra
   das camadas de baixo e mostra o resultado. Nenhuma tela grava arquivos. */
#ifndef TELAS_H
#define TELAS_H

/* telas_usuarios.c */
void telaCadastrarUsuario(void);
/* Devolve 1 quando o login deu certo. */
int telaLogin(void);
void telaAdministrarUsuarios(void);
void telaAlterarSenha(void);

/* telas_estoque.c */
void telaConsultarEstoque(void);
void telaAlertaEstoqueBaixo(void);
void telaManterProduto(int alterar);
void telaSituacaoProduto(void);
void telaMovimentacao(int entrada);

/* telas_relatorios.c */
void telaRelatorio(void);
void telaBackup(void);
/* Devolve 1 quando a demonstração foi criada. */
int telaCriarDemo(void);

#endif
