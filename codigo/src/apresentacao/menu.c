#include <stdio.h>
#include "apresentacao/menu.h"
#include "apresentacao/telas.h"
#include "apresentacao/entrada.h"
#include "apresentacao/mensagens.h"
#include "seguranca/acesso.h"
#include "seguranca/contas.h"

void executarMenu(void) {
    const char *opcoes[TOTAL_ACOES + 1] = {
        "Sair", "Consultar estoque", "Ver alerta", "Registrar entrada", "Registrar saida",
        "Cadastrar produto", "Alterar produto", "Inativar/reativar produto",
        "Gerar relatorio", "Fazer backup", "Administrar usuarios", "Alterar propria senha"
    };
    int opcao;
    while (!feof(stdin) && !ferror(stdin)) {
        const Usuario *u = usuarioNaPosicao(usuarioLogado());
        printf("\nESTOQUEFACIL | %s | Nivel %d\n", u->nome, u->nivel);
        for (int i = 1; i <= TOTAL_ACOES; i++)
            if (verificarPermissao(u->nivel, (Acao)i)) printf("%d %s\n", i, opcoes[i]);
        puts("0 Sair");
        if (!lerInteiro("Opcao: ", 0, TOTAL_ACOES, &opcao)) continue;
        if (opcao == 0) break;
        if (!permitir((Acao)opcao)) continue;
        switch ((Acao)opcao) {
            case ACAO_CONSULTAR: telaConsultarEstoque(); break;
            case ACAO_ALERTA: telaAlertaEstoqueBaixo(); break;
            case ACAO_ENTRADA: telaMovimentacao(1); break;
            case ACAO_SAIDA: telaMovimentacao(0); break;
            case ACAO_CADASTRAR_PRODUTO: telaManterProduto(0); break;
            case ACAO_ALTERAR_PRODUTO: telaManterProduto(1); break;
            case ACAO_SITUACAO_PRODUTO: telaSituacaoProduto(); break;
            case ACAO_RELATORIO: telaRelatorio(); break;
            case ACAO_BACKUP: telaBackup(); break;
            case ACAO_USUARIOS: telaAdministrarUsuarios(); break;
            case ACAO_PROPRIA_SENHA: telaAlterarSenha(); break;
        }
    }
    puts("Sessao encerrada.");
}
