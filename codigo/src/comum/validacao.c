#include <stdio.h>
#include <string.h>
#include "comum/validacao.h"

int normalizarIdentificador(char *texto, int codigo) {
    if (texto[0] == '\0') return 0;
    for (size_t i = 0; texto[i] != '\0'; i++) {
        char c = texto[i];
        if (c >= 'A' && c <= 'Z') c = (char)(c - 'A' + 'a');
        if (!((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_' || (codigo && c == '-'))) return 0;
        if (codigo && c >= 'a' && c <= 'z') c = (char)(c - 'a' + 'A');
        texto[i] = c;
    }
    return 1;
}

int dataValida(const char *data) {
    int ano, mes, dia, dias[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
    if (strlen(data) != 10 || data[4] != '-' || data[7] != '-') return 0;
    for (int i = 0; i < 10; i++)
        if (i != 4 && i != 7 && (data[i] < '0' || data[i] > '9')) return 0;
    if (sscanf(data, "%4d-%2d-%2d", &ano, &mes, &dia) != 3 || ano < 1 || mes < 1 || mes > 12) return 0;
    if (ano % 400 == 0 || (ano % 4 == 0 && ano % 100 != 0)) dias[1] = 29;
    return dia >= 1 && dia <= dias[mes - 1];
}
