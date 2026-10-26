/* EstoqueFacil: versao didatica em um arquivo C, sem SQL.
   Compilar no Mac: clang -std=c11 -Wall -Wextra estoquefacil.c -o estoquefacil
   Executar: ./estoquefacil         Demo: ./estoquefacil --demo

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
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>
#include <errno.h>
#include <time.h>
#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#include <io.h>
#else
#include <unistd.h>
#include <termios.h>
#include <signal.h>
#endif

#define MAX_USUARIOS 50
#define MAX_PRODUTOS 1000
#define MAX_MOVIMENTACOES 10000
#define TAM_SENHA 65
#define TAM_HASH 9
#define ARQUIVO "estoque_local.dat"

/* Os registros ficam em vetores. A posicao funciona como identificador. */
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

static Dados dados, antes;
static int usuarioAtual = -1;

/* 1. Leitura do teclado. O trecho de terminal fica isolado aqui. */
int lerLinha(const char *mensagem, char *texto, size_t tamanho) {
    int caractere;
    size_t comprimento;
    printf("%s", mensagem);
    fflush(stdout);
    texto[0] = '\0';
    if (fgets(texto, (int)tamanho, stdin) == NULL) return 0;
    comprimento = strlen(texto);
    if (comprimento > 0 && texto[comprimento - 1] == '\n') {
        texto[--comprimento] = '\0';
    } else {
        /* Confere o próximo caractere para aceitar o limite exato. */
        caractere = getchar();
        if (caractere != '\n' && caractere != EOF) {
            while (caractere != '\n' && caractere != EOF) caractere = getchar();
            puts("Entrada muito longa. Operacao cancelada.");
            texto[0] = '\0';
            return 0;
        }
    }
    if (comprimento > 0 && texto[comprimento - 1] == '\r') texto[--comprimento] = '\0';
    /* Evita caracteres de controle, como comandos de cor no terminal. */
    for (size_t i = 0; i < comprimento; i++) {
        unsigned char c = (unsigned char)texto[i];
        if (c < 32 || c == 127) {
            puts("Use texto sem caracteres de controle.");
            return 0;
        }
    }
    return 1;
}

int lerTexto(const char *mensagem, char *texto, size_t tamanho) {
    if (!lerLinha(mensagem, texto, tamanho)) return 0;
    /* Um campo obrigatório não aceita somente espaços. */
    for (size_t i = 0; texto[i] != '\0'; i++) {
        if (texto[i] != ' ') return 1;
    }
    puts("Campo vazio. Operacao cancelada.");
    return 0;
}

int lerInteiro(const char *mensagem, int minimo, int maximo, int *valor) {
    char texto[64], *fim;
    long numero;
    if (!lerLinha(mensagem, texto, sizeof texto)) return 0;
    errno = 0;
    numero = strtol(texto, &fim, 10);
    if (errno != 0 || fim == texto || *fim != '\0' || numero < minimo || numero > maximo) {
        puts("Numero invalido. Use um inteiro dentro do intervalo indicado.");
        return 0;
    }
    *valor = (int)numero;
    return 1;
}

#ifndef _WIN32
/* O sinal apenas avisa a leitura. A restauração ocorre no fluxo normal. */
static volatile sig_atomic_t sinalRecebido = 0;
static void avisarInterrupcao(int sinal) { sinalRecebido = sinal; }
#endif

int lerSenha(const char *mensagem, char *senha, size_t tamanho) {
#ifdef _WIN32
    size_t usados = 0;
    int caractere, excedeu = 0;
    if (!_isatty(_fileno(stdin))) return lerLinha(mensagem, senha, tamanho);
    printf("%s", mensagem);
    fflush(stdout);
    while ((caractere = _getch()) != '\r' && caractere != '\n') {
        if (caractere == 3 || caractere == 26) { puts(""); return 0; }
        if (caractere == 0 || caractere == 224) { (void)_getch(); continue; }
        if (caractere == 8) { if (usados > 0) usados--; continue; }
        if (caractere < 32 || caractere > 126) continue;
        if (usados + 1 < tamanho) senha[usados++] = (char)caractere;
        else excedeu = 1;
    }
    senha[usados] = '\0';
    puts("");
    if (excedeu) puts("Senha muito longa. Operacao cancelada.");
    return !excedeu;
#else
    struct termios original, semEco;
    struct sigaction tratamento, anteriores[5];
    int sinais[5] = {SIGINT, SIGTERM, SIGHUP, SIGQUIT, SIGTSTP};
    int instalados = 0, resultado, restaurou, sinal;
    if (!isatty(STDIN_FILENO)) return lerLinha(mensagem, senha, tamanho);
    if (tcgetattr(STDIN_FILENO, &original) != 0) {
        puts("Nao foi possivel ocultar a senha. Operacao cancelada.");
        return 0;
    }
    memset(&tratamento, 0, sizeof tratamento);
    tratamento.sa_handler = avisarInterrupcao;
    sigemptyset(&tratamento.sa_mask);
    /* Sem SA_RESTART: Ctrl+C interrompe fgets e permite restaurar o eco. */
    sinalRecebido = 0;
    for (int i = 0; i < 5; i++) {
        if (sigaction(sinais[i], &tratamento, &anteriores[i]) != 0) break;
        instalados++;
    }
    semEco = original;
    semEco.c_lflag &= (tcflag_t)~(ECHO | ECHONL);
    if (instalados != 5 || tcsetattr(STDIN_FILENO, TCSANOW, &semEco) != 0) {
        for (int i = 0; i < instalados; i++) sigaction(sinais[i], &anteriores[i], NULL);
        puts("Nao foi possivel preparar o terminal. Operacao cancelada.");
        return 0;
    }
    resultado = sinalRecebido == 0 ? lerLinha(mensagem, senha, tamanho) : 0;
    do { restaurou = tcsetattr(STDIN_FILENO, TCSANOW, &original); }
    while (restaurou != 0 && errno == EINTR);
    sinal = sinalRecebido;
    for (int i = 0; i < instalados; i++) sigaction(sinais[i], &anteriores[i], NULL);
    puts("");
    if (restaurou != 0) {
        puts("Falha ao restaurar terminal. Execute stty echo no terminal.");
        resultado = 0;
    }
    if (sinal != 0) { clearerr(stdin); raise(sinal); return 0; }
    return resultado;
#endif
}

int novaSenha(char *senha) {
    char repetida[TAM_SENHA];
    if (!lerSenha("Nova senha (8 a 64 caracteres ASCII): ", senha, TAM_SENHA)) return 0;
    if (strlen(senha) < 8) { puts("A senha precisa de pelo menos 8 caracteres."); return 0; }
    for (size_t i = 0; senha[i] != '\0'; i++) {
        if ((unsigned char)senha[i] < 32 || (unsigned char)senha[i] > 126) {
            puts("Use letras sem acento, numeros, espacos ou simbolos ASCII na senha.");
            return 0;
        }
    }
    if (!lerSenha("Repita a nova senha: ", repetida, sizeof repetida)) return 0;
    if (strcmp(senha, repetida) != 0) { puts("As senhas nao conferem."); return 0; }
    return 1;
}

int confirmar(const char *mensagem) {
    char resposta[8];
    return lerLinha(mensagem, resposta, sizeof resposta) &&
        (strcmp(resposta, "s") == 0 || strcmp(resposta, "S") == 0);
}

int normalizarIdentificador(char *texto, int codigo) {
    if (texto[0] == '\0') return 0;
    for (size_t i = 0; texto[i] != '\0'; i++) {
        char c = texto[i];
        if (c >= 'A' && c <= 'Z') c = (char)(c - 'A' + 'a');
        if (!((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_' || (codigo && c == '-'))) {
            puts("Identificador invalido. Use letras sem acento, numeros ou sublinhado.");
            return 0;
        }
        if (codigo && c >= 'a' && c <= 'z') c = (char)(c - 'a' + 'A');
        texto[i] = c;
    }
    return 1;
}

void calcularHash(const char *senha, char hash[TAM_HASH]) {
    /* Hash didático DJB2: multiplica por 33 e soma cada caractere.
       NÃO é um algoritmo seguro para senhas reais. Não possui salt.
       uint32_t mantém o mesmo resultado no macOS e no Windows. */
    uint32_t valor = 5381u;
    for (size_t i = 0; senha[i] != '\0'; i++) valor = valor * 33u + (unsigned char)senha[i];
    snprintf(hash, TAM_HASH, "%08lx", (unsigned long)valor);
}


/* 2. Arquivo local: fread carrega os vetores; fwrite guarda os vetores. */
int salvarArquivo(const char *nome) {
    char temporario[80];
    int ok;
    FILE *arquivo;
    snprintf(temporario, sizeof temporario, "%s.tmp", nome);
    arquivo = fopen(temporario, "wb");
    if (arquivo == NULL) return 0;
    ok = fwrite(&dados, sizeof dados, 1, arquivo) == 1;
    if (fclose(arquivo) != 0) ok = 0;
    /* Só substitui o arquivo anterior depois de terminar a escrita. */
    if (ok) {
#ifdef _WIN32
        ok = MoveFileExA(temporario, nome, MOVEFILE_REPLACE_EXISTING) != 0;
#else
        ok = rename(temporario, nome) == 0;
#endif
    }
    if (!ok) remove(temporario);
    return ok;
}

int guardarAlteracao(void) {
    if (salvarArquivo(ARQUIVO)) return 1;
    dados = antes;
    puts("Nao foi possivel salvar. A alteracao foi desfeita.");
    return 0;
}

int carregarDados(void) {
    FILE *arquivo = fopen(ARQUIVO, "rb");
    int ok;
    if (arquivo == NULL) {
        if (errno != ENOENT) return 0;
        memcpy(dados.versao, "EFLOCAL1", 8);
        return 1;
    }
    ok = fread(&dados, sizeof dados, 1, arquivo) == 1;
    if (fgetc(arquivo) != EOF || ferror(arquivo)) ok = 0;
    fclose(arquivo);
    if (!ok || memcmp(dados.versao, "EFLOCAL1", 8) != 0) return 0;
    if (dados.totalUsuarios < 1 || dados.totalUsuarios > MAX_USUARIOS ||
        dados.totalProdutos < 0 || dados.totalProdutos > MAX_PRODUTOS ||
        dados.totalMovimentacoes < 0 || dados.totalMovimentacoes > MAX_MOVIMENTACOES) return 0;
    /* Confere os limites antes de usar um arquivo que veio do disco. */
    for (int i = 0; i < dados.totalUsuarios; i++) {
        Usuario *u = &dados.usuarios[i];
        if (u->nivel < 1 || u->nivel > 4 || u->ativo < 0 || u->ativo > 1 || u->erros < 0 || u->erros > 3 ||
            !memchr(u->nome, 0, sizeof u->nome) || !memchr(u->login, 0, sizeof u->login) ||
            !memchr(u->senhaHash, 0, sizeof u->senhaHash)) return 0;
    }
    for (int i = 0; i < dados.totalProdutos; i++) {
        Produto *p = &dados.produtos[i];
        if (p->quantidade < 0 || p->minimo < 0 || p->ativo < 0 || p->ativo > 1 ||
            !memchr(p->codigo, 0, sizeof p->codigo) || !memchr(p->nome, 0, sizeof p->nome) ||
            !memchr(p->categoria, 0, sizeof p->categoria) || !memchr(p->fornecedor, 0, sizeof p->fornecedor)) return 0;
    }
    for (int i = 0; i < dados.totalMovimentacoes; i++) {
        Movimentacao *m = &dados.movimentacoes[i];
        if (m->produto < 0 || m->produto >= dados.totalProdutos || m->usuario < 0 || m->usuario >= dados.totalUsuarios ||
            m->quantidade < 1 || (m->tipo != 'E' && m->tipo != 'S') || !memchr(m->data, 0, sizeof m->data)) return 0;
    }
    return 1;
}

/* 3. Login e permissões. Os números abaixo são os mesmos do menu. */
int verificarPermissao(int nivel, int acao) {
    if (nivel < 1 || nivel > 4) return 0;
    switch (acao) {
        case 1: case 2: case 11: return 1;
        case 3: case 4: return nivel <= 3;
        case 5: case 6: case 7: case 9: return nivel <= 2;
        case 8: return nivel == 1 || nivel == 2 || nivel == 4;
        case 10: return nivel == 1;
        default: return 0;
    }
}

int permitir(int acao) {
    if (usuarioAtual >= 0 && dados.usuarios[usuarioAtual].ativo &&
        verificarPermissao(dados.usuarios[usuarioAtual].nivel, acao)) return 1;
    puts("Acesso negado para este nivel.");
    return 0;
}

int buscarUsuario(const char *login) {
    for (int i = 0; i < dados.totalUsuarios; i++)
        if (strcmp(dados.usuarios[i].login, login) == 0) return i;
    return -1;
}

void cadastrarUsuario(void) {
    Usuario novo = {0};
    char senha[TAM_SENHA];
    int primeiro = dados.totalUsuarios == 0;
    if (!primeiro && !permitir(10)) return;
    if (dados.totalUsuarios == MAX_USUARIOS) { puts("Limite de usuarios atingido."); return; }
    if (!lerTexto("Nome: ", novo.nome, sizeof novo.nome) ||
        !lerTexto("Login: ", novo.login, sizeof novo.login) || !normalizarIdentificador(novo.login, 0)) return;
    if (buscarUsuario(novo.login) >= 0) { puts("Login ja existente."); return; }
    novo.nivel = 1;
    if (!primeiro && !lerInteiro("Nivel (1 Admin, 2 Gerente, 3 Operador, 4 Consulta): ", 1, 4, &novo.nivel)) return;
    if (!novaSenha(senha)) return;
    calcularHash(senha, novo.senhaHash);
    novo.ativo = 1;
    antes = dados;
    dados.usuarios[dados.totalUsuarios++] = novo;
    if (guardarAlteracao()) puts("Usuario cadastrado.");
}

int fazerLogin(void) {
    char login[30], senha[TAM_SENHA], hash[TAM_HASH];
    for (int tentativa = 1; tentativa <= 3; tentativa++) {
        int posicao, correto;
        long long agora = (long long)time(NULL);
        Usuario *u;
        printf("\nTentativa %d de 3. Enter no login encerra.\n", tentativa);
        if (!lerLinha("Login: ", login, sizeof login) || login[0] == '\0') return -1;
        if (!normalizarIdentificador(login, 0)) continue;
        if (!lerSenha("Senha: ", senha, sizeof senha)) return -1;
        posicao = buscarUsuario(login);
        if (posicao < 0) { puts("Login ou senha incorretos."); continue; }
        u = &dados.usuarios[posicao];
        if (!u->ativo) { puts("Usuario inativo."); continue; }
        agora = (long long)time(NULL);
        if (u->bloqueadoAte > agora) { puts("Usuario bloqueado. Aguarde cinco minutos desde o terceiro erro."); continue; }
        antes = dados;
        if (u->bloqueadoAte != 0) { u->erros = 0; u->bloqueadoAte = 0; }
        calcularHash(senha, hash);
        correto = strcmp(hash, u->senhaHash) == 0;
        if (correto) u->erros = 0;
        else {
            u->erros++;
            if (u->erros == 3) u->bloqueadoAte = agora + 300;
        }
        if (!guardarAlteracao()) return -1;
        if (correto) return posicao;
        puts(u->erros == 3 ? "Tres erros: conta bloqueada por cinco minutos." : "Login ou senha incorretos.");
    }
    return -1;
}

void administrarUsuarios(void) {
    int opcao, id, nivel;
    if (!permitir(10)) return;
    for (int i = 0; i < dados.totalUsuarios; i++)
        printf("%d | %s | nivel %d | %s\n", i + 1, dados.usuarios[i].nome,
            dados.usuarios[i].nivel, dados.usuarios[i].ativo ? "ativo" : "inativo");
    if (!lerInteiro("1 Cadastrar, 2 Alterar nivel, 3 Inativar/reativar, 0 Voltar: ", 0, 3, &opcao) || opcao == 0) return;
    if (opcao == 1) { cadastrarUsuario(); return; }
    if (!lerInteiro("Numero do usuario: ", 1, dados.totalUsuarios, &id)) return;
    id--;
    /* A própria conta permanece ativa e Administrador: não se perde o acesso. */
    if (id == usuarioAtual) { puts("Nao altere o nivel ou a situacao da propria conta."); return; }
    antes = dados;
    if (opcao == 2) {
        if (!lerInteiro("Novo nivel (1 a 4): ", 1, 4, &nivel)) return;
        dados.usuarios[id].nivel = nivel;
    } else {
        if (!confirmar("Confirma inativar/reativar esta conta? (s/N): ")) return;
        dados.usuarios[id].ativo = !dados.usuarios[id].ativo;
    }
    if (guardarAlteracao()) puts("Usuario atualizado.");
}

void alterarSenha(void) {
    char senha[TAM_SENHA], hash[TAM_HASH];
    if (!permitir(11) || !lerSenha("Senha atual: ", senha, sizeof senha)) return;
    calcularHash(senha, hash);
    if (strcmp(hash, dados.usuarios[usuarioAtual].senhaHash) != 0) { puts("Senha atual incorreta."); return; }
    if (!novaSenha(senha)) return;
    antes = dados;
    calcularHash(senha, dados.usuarios[usuarioAtual].senhaHash);
    if (guardarAlteracao()) puts("Senha alterada.");
}

/* 4. Produtos, entradas, saídas e alerta. */
int buscarProduto(const char *codigo) {
    for (int i = 0; i < dados.totalProdutos; i++)
        if (strcmp(dados.produtos[i].codigo, codigo) == 0) return i;
    return -1;
}

int escolherProduto(void) {
    char codigo[20];
    int posicao;
    if (!lerTexto("Codigo do produto: ", codigo, sizeof codigo) || !normalizarIdentificador(codigo, 1)) return -1;
    posicao = buscarProduto(codigo);
    if (posicao < 0) puts("Produto nao encontrado.");
    return posicao;
}

void verificarEstoqueBaixo(void) {
    int alertas = 0;
    if (!permitir(2)) return;
    for (int i = 0; i < dados.totalProdutos; i++) {
        Produto *p = &dados.produtos[i];
        if (p->ativo && p->quantidade <= p->minimo) {
            printf("[ESTOQUE BAIXO] %s | %s | quantidade %d | minimo %d\n", p->codigo, p->nome, p->quantidade, p->minimo);
            alertas++;
        }
    }
    if (alertas == 0) puts("Nenhum produto ativo esta com estoque baixo.");
}

void consultarEstoque(void) {
    char busca[60];
    int encontrados = 0;
    if (!permitir(1) || !lerLinha("Codigo ou parte do nome (Enter para todos): ", busca, sizeof busca)) return;
    for (int i = 0; i < dados.totalProdutos; i++) {
        Produto *p = &dados.produtos[i];
        if (busca[0] == '\0' || strstr(p->codigo, busca) || strstr(p->nome, busca)) {
            printf("%s | %s | saldo %d | minimo %d | %s\n", p->codigo, p->nome,
                p->quantidade, p->minimo, p->ativo ? "ativo" : "inativo");
            encontrados++;
        }
    }
    if (!encontrados) puts("Produto nao encontrado.");
}

void manterProduto(int alterar) {
    Produto novo = {0};
    int posicao;
    if (!permitir(alterar ? 6 : 5)) return;
    if (alterar) {
        posicao = escolherProduto();
        if (posicao < 0) return;
        novo = dados.produtos[posicao];
    } else {
        if (dados.totalProdutos == MAX_PRODUTOS) { puts("Limite de produtos atingido."); return; }
        posicao = dados.totalProdutos;
        if (!lerTexto("Codigo unico: ", novo.codigo, sizeof novo.codigo) || !normalizarIdentificador(novo.codigo, 1)) return;
        if (buscarProduto(novo.codigo) >= 0) { puts("Codigo ja existente."); return; }
        novo.ativo = 1;
    }
    if (!lerTexto("Nome: ", novo.nome, sizeof novo.nome) ||
        !lerTexto("Categoria: ", novo.categoria, sizeof novo.categoria) ||
        !lerTexto("Fornecedor principal: ", novo.fornecedor, sizeof novo.fornecedor) ||
        !lerInteiro("Estoque minimo (0 a 2147483647): ", 0, INT_MAX, &novo.minimo)) return;
    if (!confirmar("Salvar produto? (s/N): ")) return;
    antes = dados;
    dados.produtos[posicao] = novo;
    if (!alterar) dados.totalProdutos++;
    if (guardarAlteracao()) { puts("Produto salvo. Use entrada/saida para mudar o saldo."); verificarEstoqueBaixo(); }
}

void mudarSituacaoProduto(void) {
    int posicao;
    if (!permitir(7)) return;
    posicao = escolherProduto();
    if (posicao < 0 || !confirmar("Inativar/reativar? Saldo e historico serao mantidos. (s/N): ")) return;
    antes = dados;
    dados.produtos[posicao].ativo = !dados.produtos[posicao].ativo;
    if (guardarAlteracao()) { puts("Situacao atualizada."); verificarEstoqueBaixo(); }
}

void registrarMovimentacao(int entrada) {
    int posicao, quantidade;
    Movimentacao nova = {0};
    Produto *p;
    time_t agora;
    struct tm *horario;
    if (!permitir(entrada ? 3 : 4)) return;
    if (dados.totalMovimentacoes == MAX_MOVIMENTACOES) { puts("Limite do historico atingido."); return; }
    posicao = escolherProduto();
    if (posicao < 0) return;
    p = &dados.produtos[posicao];
    if (!p->ativo) { puts("Produto inativo."); return; }
    printf("Saldo atual: %d\n", p->quantidade);
    if (!lerInteiro("Quantidade (1 a 2147483647): ", 1, INT_MAX, &quantidade)) return;
    if (!entrada && quantidade > p->quantidade) { puts("Estoque insuficiente."); return; }
    if (entrada && quantidade > INT_MAX - p->quantidade) { puts("Quantidade ultrapassa o limite de um inteiro."); return; }
    if (!confirmar("Confirmar movimentacao? (s/N): ")) return;
    agora = time(NULL); horario = localtime(&agora);
    if (horario == NULL || !strftime(nova.data, sizeof nova.data, "%Y-%m-%d %H:%M:%S", horario)) return;
    nova.produto = posicao; nova.usuario = usuarioAtual; nova.quantidade = quantidade;
    nova.tipo = entrada ? 'E' : 'S';
    antes = dados;
    if (entrada) p->quantidade += quantidade;
    else p->quantidade -= quantidade;
    dados.movimentacoes[dados.totalMovimentacoes++] = nova;
    /* Saldo e histórico ficam juntos no mesmo arquivo. */
    if (guardarAlteracao()) { printf("Movimentacao salva. Novo saldo: %d\n", p->quantidade); verificarEstoqueBaixo(); }
}

/* 5. Relatório de texto e backup. Não há comandos SQL. */
int dataValida(const char *data) {
    int ano, mes, dia, dias[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
    if (strlen(data) != 10 || data[4] != '-' || data[7] != '-') return 0;
    for (int i = 0; i < 10; i++)
        if (i != 4 && i != 7 && (data[i] < '0' || data[i] > '9')) return 0;
    if (sscanf(data, "%4d-%2d-%2d", &ano, &mes, &dia) != 3 || ano < 1 || mes < 1 || mes > 12) return 0;
    if (ano % 400 == 0 || (ano % 4 == 0 && ano % 100 != 0)) dias[1] = 29;
    return dia >= 1 && dia <= dias[mes - 1];
}

void gerarRelatorio(void) {
    char codigo[20], inicio[11], fim[11];
    int tipo, ok;
    FILE *arquivo;
    if (!permitir(8) || !lerInteiro("1 Estoque, 2 Historico, 0 Voltar: ", 0, 2, &tipo) || tipo == 0) return;
    if (!lerLinha("Codigo exato (Enter para todos): ", codigo, sizeof codigo)) return;
    if (codigo[0] && !normalizarIdentificador(codigo, 1)) return;
    inicio[0] = fim[0] = '\0';
    if (tipo == 2) {
        if (!lerLinha("Data inicial AAAA-MM-DD (Enter sem limite): ", inicio, sizeof inicio) ||
            !lerLinha("Data final AAAA-MM-DD (Enter sem limite): ", fim, sizeof fim)) return;
        if ((inicio[0] && !dataValida(inicio)) || (fim[0] && !dataValida(fim)) ||
            (inicio[0] && fim[0] && strcmp(inicio, fim) > 0)) { puts("Periodo invalido."); return; }
    }
    if (!confirmar("Gerar relatorio.txt? Substitui o relatorio anterior. (s/N): ")) return;
    arquivo = fopen("relatorio.txt", "w");
    if (arquivo == NULL) { puts("Falha ao criar relatorio."); return; }
    fprintf(arquivo, "ESTOQUEFACIL - %s\n", tipo == 1 ? "ESTOQUE" : "HISTORICO");
    if (tipo == 1) {
        for (int i = 0; i < dados.totalProdutos; i++) {
            Produto *p = &dados.produtos[i];
            if (!codigo[0] || strcmp(codigo, p->codigo) == 0)
                fprintf(arquivo, "%s | %s | saldo %d | minimo %d | %s\n", p->codigo, p->nome,
                    p->quantidade, p->minimo, p->ativo ? "ativo" : "inativo");
        }
    } else {
        for (int i = 0; i < dados.totalMovimentacoes; i++) {
            Movimentacao *m = &dados.movimentacoes[i];
            if (codigo[0] && strcmp(codigo, dados.produtos[m->produto].codigo) != 0) continue;
            if (inicio[0] && strncmp(m->data, inicio, 10) < 0) continue;
            if (fim[0] && strncmp(m->data, fim, 10) > 0) continue;
            fprintf(arquivo, "%s | %s | %s | %d | %s\n", m->data, dados.produtos[m->produto].codigo,
                m->tipo == 'E' ? "ENTRADA" : "SAIDA", m->quantidade, dados.usuarios[m->usuario].nome);
        }
    }
    ok = !ferror(arquivo);
    if (fclose(arquivo) != 0) ok = 0;
    puts(ok ? "Relatorio salvo em relatorio.txt. Abra esse arquivo para consultar." : "Falha na gravacao do relatorio.");
}

void fazerBackup(void) {
    if (!permitir(9) || !confirmar("Salvar backup_local.dat? Substitui o backup anterior. (s/N): ")) return;
    puts(salvarArquivo("backup_local.dat") ? "Backup salvo." : "Falha no backup. A copia anterior foi mantida.");
}

int criarDemo(void) {
    const char *logins[4] = {"admin", "gerente", "operador", "consulta"};
    if (dados.totalUsuarios != 0) { puts("Ja existem dados. Execute sem --demo."); return 0; }
    antes = dados;
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
    for (int i = 0; i < 2; i++) {
        time_t agora = time(NULL);
        struct tm *horario = localtime(&agora);
        if (horario == NULL) { dados = antes; return 0; }
        dados.movimentacoes[i].produto = i;
        dados.movimentacoes[i].usuario = 0;
        dados.movimentacoes[i].tipo = 'E';
        dados.movimentacoes[i].quantidade = dados.produtos[i].quantidade;
        strftime(dados.movimentacoes[i].data, sizeof dados.movimentacoes[i].data, "%Y-%m-%d %H:%M:%S", horario);
    }
    dados.totalMovimentacoes = 2;
    if (!guardarAlteracao()) return 0;
    puts("Demo criada. Logins: admin, gerente, operador, consulta. Senha de todos: teste123");
    return 1;
}

int main(int argc, char *argv[]) {
    const char *opcoes[12] = {
        "Sair", "Consultar estoque", "Ver alerta", "Registrar entrada", "Registrar saida",
        "Cadastrar produto", "Alterar produto", "Inativar/reativar produto",
        "Gerar relatorio", "Fazer backup", "Administrar usuarios", "Alterar propria senha"
    };
    int opcao;
    if (argc > 2 || (argc == 2 && strcmp(argv[1], "--demo") != 0)) { puts("Uso: ./estoquefacil [--demo]"); return 1; }
    if (!carregarDados()) { puts("Arquivo local invalido ou sem acesso. Preserve-o e confira seu backup."); return 1; }
    if (argc == 2 && !criarDemo()) return 1;
    if (dados.totalUsuarios == 0) { puts("Crie o primeiro Administrador:"); cadastrarUsuario(); }
    if (dados.totalUsuarios == 0) return 1;
    usuarioAtual = fazerLogin();
    if (usuarioAtual < 0) return 1;
    verificarEstoqueBaixo();
    while (!feof(stdin) && !ferror(stdin)) {
        printf("\nESTOQUEFACIL | %s | Nivel %d\n", dados.usuarios[usuarioAtual].nome, dados.usuarios[usuarioAtual].nivel);
        for (int i = 1; i <= 11; i++)
            if (verificarPermissao(dados.usuarios[usuarioAtual].nivel, i)) printf("%d %s\n", i, opcoes[i]);
        puts("0 Sair");
        if (!lerInteiro("Opcao: ", 0, 11, &opcao)) continue;
        if (opcao == 0) break;
        if (!permitir(opcao)) continue;
        switch (opcao) {
            case 1: consultarEstoque(); break;
            case 2: verificarEstoqueBaixo(); break;
            case 3: registrarMovimentacao(1); break;
            case 4: registrarMovimentacao(0); break;
            case 5: manterProduto(0); break;
            case 6: manterProduto(1); break;
            case 7: mudarSituacaoProduto(); break;
            case 8: gerarRelatorio(); break;
            case 9: fazerBackup(); break;
            case 10: administrarUsuarios(); break;
            case 11: alterarSenha(); break;
        }
    }
    puts("Sessao encerrada.");
    return 0;
}
