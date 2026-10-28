/* Validações de texto usadas por mais de uma camada. Não acessam dados nem tela. */
#ifndef VALIDACAO_H
#define VALIDACAO_H

/* Login: minúsculas, números e sublinhado. Código: maiúsculas e também hífen.
   Converte o texto no próprio vetor. Devolve 0 quando há caractere inválido. */
int normalizarIdentificador(char *texto, int codigo);

/* Confere uma data AAAA-MM-DD, inclusive anos bissextos. */
int dataValida(const char *data);

#endif
