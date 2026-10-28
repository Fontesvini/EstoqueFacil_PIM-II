# Código do EstoqueFácil

O programa fica em [src/](src), dividido em camadas. Cada camada é uma pasta, e cada assunto tem um arquivo `.c` com o seu `.h`. Para compilar e abrir no Mac, o script [rodar.sh](rodar.sh) confere as ferramentas, compila todos os arquivos e executa o programa.

Com esta pasta aberta no terminal do VS Code:

```sh
bash rodar.sh --demo
```

Login `admin`, senha `teste123`, apenas para teste. Nas próximas execuções, use `bash rodar.sh`. A demonstração só é criada quando ainda não há dados.

## Arquitetura em camadas

```text
src/
├── main.c                  # Ponto de partida: carrega dados, login e menu
├── apresentacao/           # 1. Apresentação: o que aparece na tela
│   ├── menu.c              #    menu principal, só com as opções permitidas
│   ├── telas_usuarios.c    #    primeiro cadastro, login, usuários e senha
│   ├── telas_estoque.c     #    consulta, alerta, produtos, entradas e saídas
│   ├── telas_relatorios.c  #    relatório, backup e demonstração
│   ├── entrada.c           #    leitura do teclado e senha sem eco
│   └── mensagens.c         #    texto de cada Resultado
├── negocio/                # 2. Regras de negócio
│   ├── produtos.c          #    cadastro, alteração, inativação e alerta
│   ├── movimentacoes.c     #    entradas e saídas com data, hora e usuário
│   ├── relatorios.c        #    relatorio.txt e backup
│   └── demo.c              #    dados de exemplo
├── seguranca/              # 3. Autenticação e controle de acesso
│   ├── acesso.c            #    login, bloqueio, sessão e permissões
│   └── contas.c            #    usuários, hash e regras da senha
├── dados/                  # 4. Acesso a dados
│   └── repositorio.c       #    estoque_local.dat, desfazer e backup
└── comum/                  # Usado por todas as camadas
    ├── modelo.h            #    structs Usuario, Produto, Movimentacao e Dados
    ├── resultado.h         #    códigos de sucesso e de erro
    └── validacao.c         #    identificadores e datas
```

A regra é simples: **uma camada só chama as camadas de baixo**. A apresentação pede os dados e mostra mensagens. As regras de negócio decidem se a operação é válida. A segurança confere quem está usando e o que pode fazer. Só a camada de dados abre arquivos.

As regras não escrevem na tela. Elas devolvem um `Resultado`, como `OK` ou `ERRO_ESTOQUE_INSUFICIENTE`, e a apresentação mostra a mensagem correspondente. Assim, uma regra pode ser testada ou reaproveitada sem mudar os menus.

## Para estudar

Comece por [main.c](src/main.c). Depois acompanhe uma operação de cima para baixo. Uma saída de estoque, por exemplo, passa por:

1. `telaMovimentacao` em [telas_estoque.c](src/apresentacao/telas_estoque.c): pede código, quantidade e confirmação.
2. `registrarMovimentacao` em [movimentacoes.c](src/negocio/movimentacoes.c): confere permissão, produto ativo e saldo.
3. `autorizar` em [acesso.c](src/seguranca/acesso.c): confere o nível do usuário da sessão.
4. `confirmarAlteracao` em [repositorio.c](src/dados/repositorio.c): grava o arquivo ou desfaz a alteração.

Os arquivos `.h` de cada pasta resumem o que a camada oferece. A [etapa de Programação em C](../docs/trabalho/05-programacao-c.md) comenta os trechos principais. O [guia completo](LEIA_ME.md) explica instalação, uso, testes e limitações.

Os arquivos `.dat`, o executável e o relatório são gerados durante o uso e estão no `.gitignore`. Eles não acompanham o repositório.

[Voltar ao início](../README.md)
