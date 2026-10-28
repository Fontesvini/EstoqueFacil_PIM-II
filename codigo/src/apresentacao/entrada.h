/* Camada de apresentação: leitura do teclado (RNF01).
   O trecho específico de terminal fica isolado em entrada.c. */
#ifndef ENTRADA_H
#define ENTRADA_H

#include <stddef.h>

/* Todas devolvem 0 quando a leitura é cancelada ou inválida, já com aviso na tela. */
int lerLinha(const char *mensagem, char *texto, size_t tamanho);
int lerTexto(const char *mensagem, char *texto, size_t tamanho);
int lerInteiro(const char *mensagem, int minimo, int maximo, int *valor);
/* Oculta a senha durante a digitação: termios no Mac; conio.h no Windows. */
int lerSenha(const char *mensagem, char *senha, size_t tamanho);
/* Aceita somente s ou S. */
int confirmar(const char *mensagem);

#endif
