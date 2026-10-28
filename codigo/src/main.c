/* EstoqueFacil: versao didatica em C, sem SQL, organizada em camadas.
   Compilar no Mac: bash rodar.sh   (ou o comando clang do LEIA_ME)
   Executar: ./estoquefacil         Demo: ./estoquefacil --demo

   Camadas (cada uma usa somente as de baixo):
     apresentacao/  menus, telas, leitura do teclado e mensagens
     negocio/       produtos, movimentacoes, alerta, relatorios e demo
     seguranca/     login, sessao, permissoes, contas e senhas
     dados/         arquivo local estoque_local.dat e backup
     comum/         structs, codigos de resultado e validacoes (todas usam)

   RF01: cadastrar usuarios, fazer login e controlar os quatro niveis.
   RF02: cadastrar, alterar e inativar produtos.
   RF03: registrar entradas e saidas com data, hora e usuario.
   RF04: mostrar alerta quando quantidade <= minimo.
   RF05: consultar estoque, gerar relatorio TXT e fazer backup.

   RNF01: usabilidade, com menus numerados e mensagens simples.
   RNF02: desempenho, com ate 1000 produtos; medir as consultas.
   RNF03: seguranca, com hash didatico, permissoes e bloqueio no login.
   RNF04: confiabilidade, com arquivo local e copia de seguranca.
   RNF05: compatibilidade, com C no Mac/Windows e uso sem internet.
   O LEIA_ME explica as limitacoes e o que foi testado.
*/
#include <stdio.h>
#include <string.h>
#include "apresentacao/menu.h"
#include "apresentacao/telas.h"
#include "seguranca/contas.h"
#include "dados/repositorio.h"

int main(int argc, char *argv[]) {
    if (argc > 2 || (argc == 2 && strcmp(argv[1], "--demo") != 0)) { puts("Uso: ./estoquefacil [--demo]"); return 1; }
    if (!carregarDados()) { puts("Arquivo local invalido ou sem acesso. Preserve-o e confira seu backup."); return 1; }
    if (argc == 2 && !telaCriarDemo()) return 1;
    if (quantidadeUsuarios() == 0) { puts("Crie o primeiro Administrador:"); telaCadastrarUsuario(); }
    if (quantidadeUsuarios() == 0) return 1;
    if (!telaLogin()) return 1;
    telaAlertaEstoqueBaixo();
    executarMenu();
    return 0;
}
