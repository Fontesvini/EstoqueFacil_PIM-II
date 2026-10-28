#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#include <io.h>
#else
#include <unistd.h>
#include <termios.h>
#include <signal.h>
#endif
#include "apresentacao/entrada.h"

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

int confirmar(const char *mensagem) {
    char resposta[8];
    return lerLinha(mensagem, resposta, sizeof resposta) &&
        (strcmp(resposta, "s") == 0 || strcmp(resposta, "S") == 0);
}
