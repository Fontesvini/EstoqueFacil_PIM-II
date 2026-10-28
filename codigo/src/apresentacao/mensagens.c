#include <stdio.h>
#include "apresentacao/mensagens.h"
#include "seguranca/acesso.h"

const char *textoDoResultado(Resultado resultado) {
    switch (resultado) {
        case OK: return "";
        case ERRO_ACESSO_NEGADO: return "Acesso negado para este nivel.";
        case ERRO_IDENTIFICADOR: return "Identificador invalido. Use letras sem acento, numeros ou sublinhado.";
        case ERRO_OPCAO: return "Dados invalidos. Operacao cancelada.";
        case ERRO_LIMITE_USUARIOS: return "Limite de usuarios atingido.";
        case ERRO_LOGIN_EXISTENTE: return "Login ja existente.";
        case ERRO_USUARIO_INEXISTENTE: return "Usuario nao encontrado.";
        case ERRO_PROPRIA_CONTA: return "Nao altere o nivel ou a situacao da propria conta.";
        case ERRO_SENHA_CURTA: return "A senha precisa de pelo menos 8 caracteres.";
        case ERRO_SENHA_CARACTERES: return "Use letras sem acento, numeros, espacos ou simbolos ASCII na senha.";
        case ERRO_SENHA_ATUAL: return "Senha atual incorreta.";
        case ERRO_LOGIN_INCORRETO: return "Login ou senha incorretos.";
        case ERRO_USUARIO_INATIVO: return "Usuario inativo.";
        case ERRO_USUARIO_BLOQUEADO: return "Usuario bloqueado. Aguarde cinco minutos desde o terceiro erro.";
        case ERRO_BLOQUEIO_INICIADO: return "Tres erros: conta bloqueada por cinco minutos.";
        case ERRO_LIMITE_PRODUTOS: return "Limite de produtos atingido.";
        case ERRO_CODIGO_EXISTENTE: return "Codigo ja existente.";
        case ERRO_PRODUTO_INEXISTENTE: return "Produto nao encontrado.";
        case ERRO_PRODUTO_INATIVO: return "Produto inativo.";
        case ERRO_LIMITE_HISTORICO: return "Limite do historico atingido.";
        case ERRO_QUANTIDADE: return "Quantidade invalida.";
        case ERRO_ESTOQUE_INSUFICIENTE: return "Estoque insuficiente.";
        case ERRO_ESTOURO: return "Quantidade ultrapassa o limite de um inteiro.";
        case ERRO_DATA_HORA: return "Nao foi possivel obter a data e a hora.";
        case ERRO_PERIODO: return "Periodo invalido.";
        case ERRO_RELATORIO_CRIAR: return "Falha ao criar relatorio.";
        case ERRO_RELATORIO_GRAVAR: return "Falha na gravacao do relatorio.";
        case ERRO_BACKUP: return "Falha no backup. A copia anterior foi mantida.";
        case ERRO_GRAVACAO: return "Nao foi possivel salvar. A alteracao foi desfeita.";
        case ERRO_DADOS_EXISTENTES: return "Ja existem dados. Execute sem --demo.";
    }
    return "Erro desconhecido.";
}

int mostrarResultado(Resultado resultado) {
    if (resultado == OK) return 1;
    puts(textoDoResultado(resultado));
    return 0;
}

int permitir(Acao acao) {
    return mostrarResultado(autorizar(acao) ? OK : ERRO_ACESSO_NEGADO);
}
