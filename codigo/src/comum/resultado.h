/* Resultado: código devolvido pelas camadas de baixo.
   As regras não escrevem na tela; a apresentação traduz o código em mensagem. */
#ifndef RESULTADO_H
#define RESULTADO_H

typedef enum {
    OK = 0,
    ERRO_ACESSO_NEGADO,
    ERRO_IDENTIFICADOR,
    ERRO_OPCAO,
    /* Usuários e login */
    ERRO_LIMITE_USUARIOS,
    ERRO_LOGIN_EXISTENTE,
    ERRO_USUARIO_INEXISTENTE,
    ERRO_PROPRIA_CONTA,
    ERRO_SENHA_CURTA,
    ERRO_SENHA_CARACTERES,
    ERRO_SENHA_ATUAL,
    ERRO_LOGIN_INCORRETO,
    ERRO_USUARIO_INATIVO,
    ERRO_USUARIO_BLOQUEADO,
    ERRO_BLOQUEIO_INICIADO,
    /* Produtos e movimentações */
    ERRO_LIMITE_PRODUTOS,
    ERRO_CODIGO_EXISTENTE,
    ERRO_PRODUTO_INEXISTENTE,
    ERRO_PRODUTO_INATIVO,
    ERRO_LIMITE_HISTORICO,
    ERRO_QUANTIDADE,
    ERRO_ESTOQUE_INSUFICIENTE,
    ERRO_ESTOURO,
    ERRO_DATA_HORA,
    /* Relatórios, backup e arquivo */
    ERRO_PERIODO,
    ERRO_RELATORIO_CRIAR,
    ERRO_RELATORIO_GRAVAR,
    ERRO_BACKUP,
    ERRO_GRAVACAO,
    ERRO_DADOS_EXISTENTES
} Resultado;

#endif
