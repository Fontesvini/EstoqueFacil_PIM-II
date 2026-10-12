# Código do EstoqueFácil

O programa inteiro está em [estoquefacil.c](estoquefacil.c). Para compilar e abrir no Mac, o script [rodar.sh](rodar.sh) confere as ferramentas e executa o programa.

Com esta pasta aberta no terminal do VS Code:

```sh
bash rodar.sh --demo
```

Login `admin`, senha `teste123`, apenas para teste. Nas próximas execuções, use `bash rodar.sh`. A demonstração só é criada quando ainda não há dados.

Leia o [guia completo de instalação, uso e testes](LEIA_ME.md). Ele explica o primeiro cadastro, os quatro níveis, as opções para Windows e as limitações do arquivo local.

Para estudar, comece pelo `main`, no fim do arquivo, e acompanhe `fazerLogin`, `verificarPermissao`, `manterProduto` e `registrarMovimentacao`. A [etapa de Programação em C](../docs/trabalho/05-programacao-c.md) comenta os trechos principais.

Os arquivos `.dat`, o executável e o relatório são gerados durante o uso e estão no `.gitignore`. Eles não acompanham o repositório.

[Voltar ao início](../README.md)
