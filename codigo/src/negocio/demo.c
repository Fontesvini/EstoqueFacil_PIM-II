#include <stdio.h>
#include <time.h>
#include "negocio/demo.h"
#include "seguranca/contas.h"
#include "dados/repositorio.h"

Resultado criarDemo(void) {
    const char *logins[4] = {"admin", "gerente", "operador", "consulta"};
    if (dados.totalUsuarios != 0) return ERRO_DADOS_EXISTENTES;
    iniciarAlteracao();
    for (int i = 0; i < 4; i++) {
        snprintf(dados.usuarios[i].nome, sizeof dados.usuarios[i].nome, "%s", logins[i]);
        snprintf(dados.usuarios[i].login, sizeof dados.usuarios[i].login, "%s", logins[i]);
        calcularHash("teste123", dados.usuarios[i].senhaHash);
        dados.usuarios[i].nivel = i + 1; dados.usuarios[i].ativo = 1;
    }
    dados.totalUsuarios = 4;
    dados.produtos[0] = (Produto){"CAN001", "Caneta azul", "Escrita", "Distribuidora Modelo", 3, 5, 1};
    dados.produtos[1] = (Produto){"CAD001", "Caderno", "Papel", "Distribuidora Modelo", 10, 3, 1};
    dados.totalProdutos = 2;
    /* O saldo inicial também aparece no histórico, como entrada do admin. */
    for (int i = 0; i < 2; i++) {
        time_t agora = time(NULL);
        struct tm *horario = localtime(&agora);
        if (horario == NULL) { desfazerAlteracao(); return ERRO_DATA_HORA; }
        dados.movimentacoes[i].produto = i;
        dados.movimentacoes[i].usuario = 0;
        dados.movimentacoes[i].tipo = 'E';
        dados.movimentacoes[i].quantidade = dados.produtos[i].quantidade;
        strftime(dados.movimentacoes[i].data, sizeof dados.movimentacoes[i].data, "%Y-%m-%d %H:%M:%S", horario);
    }
    dados.totalMovimentacoes = 2;
    return confirmarAlteracao() ? OK : ERRO_GRAVACAO;
}
