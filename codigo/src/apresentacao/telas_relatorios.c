#include <stdio.h>
#include "apresentacao/telas.h"
#include "apresentacao/entrada.h"
#include "apresentacao/mensagens.h"
#include "negocio/relatorios.h"
#include "negocio/demo.h"
#include "comum/validacao.h"

void telaRelatorio(void) {
    char codigo[20], inicio[11], fim[11];
    int tipo;
    if (!permitir(ACAO_RELATORIO) || !lerInteiro("1 Estoque, 2 Historico, 0 Voltar: ", 0, 2, &tipo) || tipo == 0) return;
    if (!lerLinha("Codigo exato (Enter para todos): ", codigo, sizeof codigo)) return;
    if (codigo[0] && !normalizarIdentificador(codigo, 1)) { mostrarResultado(ERRO_IDENTIFICADOR); return; }
    inicio[0] = fim[0] = '\0';
    if (tipo == RELATORIO_HISTORICO) {
        if (!lerLinha("Data inicial AAAA-MM-DD (Enter sem limite): ", inicio, sizeof inicio) ||
            !lerLinha("Data final AAAA-MM-DD (Enter sem limite): ", fim, sizeof fim)) return;
        if (!mostrarResultado(validarPeriodo(inicio, fim))) return;
    }
    if (!confirmar("Gerar relatorio.txt? Substitui o relatorio anterior. (s/N): ")) return;
    if (mostrarResultado(gerarRelatorio(tipo, codigo, inicio, fim)))
        puts("Relatorio salvo em relatorio.txt. Abra esse arquivo para consultar.");
}

void telaBackup(void) {
    if (!permitir(ACAO_BACKUP) || !confirmar("Salvar backup_local.dat? Substitui o backup anterior. (s/N): ")) return;
    if (mostrarResultado(fazerBackup())) puts("Backup salvo.");
}

int telaCriarDemo(void) {
    if (!mostrarResultado(criarDemo())) return 0;
    puts("Demo criada. Logins: admin, gerente, operador, consulta. Senha de todos: teste123");
    return 1;
}
