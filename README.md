# EstoqueFácil | PIM II

O EstoqueFácil é o nosso projeto do segundo semestre de Análise e Desenvolvimento de Sistemas da UNIP. É um programa em C para controlar o estoque de um pequeno comércio, com login, quatro níveis de acesso e aviso quando um produto está acabando.

Usamos uma empresa fictícia, a Papelaria Ponto do Caderno. O dono anota parte das compras em um caderno e nem sempre percebe que um produto acabou. A ideia do programa é ajudar nessa conferência: registrar o que entrou, o que saiu e o que precisa ser reposto.

O código é organizado em camadas: apresentação, regras de negócio, autenticação e controle de acesso, e acesso a dados. Cada camada fica em uma pasta, o que facilita encontrar e conferir cada parte. Os dados ficam no próprio computador, em `estoque_local.dat`. Não é necessário instalar banco de dados, e o uso do estoque não depende de internet.

## Por onde começar

| Quero... | Onde encontrar |
|---|---|
| Testar o programa | [Passo a passo do código](codigo/README.md) |
| Ler o trabalho | [PIM em Word](docs/trabalho/PIM_II_EstoqueFacil.docx) ou [capítulos no GitHub](docs/trabalho/README.md) |
| Ver os desenhos | [Índice com os nove diagramas](diagramas/README.md) |
| Entender o que foi planejado | [Backlog](gestao/BACKLOG.md) e [cronograma de dez semanas](gestao/CRONOGRAMA.md) |
| Acompanhar as reuniões | [Registros semanais, um arquivo por encontro](gestao/reunioes/README.md) |
| Conferir os testes e limites | [Verificação do projeto](docs/VERIFICACAO.md) |

## O que o programa faz

- Cadastra usuários e verifica login, senha e nível de acesso.
- Cadastra e mantém produtos, sem apagar o histórico.
- Registra entradas e saídas com data, hora e usuário.
- Mostra alerta quando o saldo é menor ou igual ao estoque mínimo.
- Consulta estoque, exporta relatórios em texto e faz backup, conforme a permissão.

Por exemplo: CAN001 tem três canetas e mínimo de cinco. O alerta aparece. Se entrarem mais três, o saldo vai para seis e o produto sai da lista. Uma tentativa de retirar sete deve ser recusada.

## Como testar no Mac

Na página do repositório, use **Code > Download ZIP** e extraia a pasta. Depois:

- **Dois cliques** em `EstoqueFacil.command`, no Finder. Na primeira vez, se o Mac pedir confirmação, use botão direito > **Abrir**.
- Ou abra a pasta no **VS Code** (a principal, `codigo` ou `src`) e aperte **F5**.
- Ou, no terminal, dentro da pasta `codigo`, execute `bash rodar.sh`.

Na primeira vez, o programa pede o cadastro do primeiro Administrador. Cada cadastro e movimentação é salvo na hora em `codigo/src/output/estoque_local.dat`, então tudo continua lá quando você abrir de novo. O relatório (`relatorio.txt`) e o backup ficam na mesma pasta `codigo/src/output`. Para testar com contas de exemplo (`admin`, `gerente`, `operador` e `consulta`, senha `teste123`), use `bash rodar.sh --demo` antes de ter dados. Se faltar o compilador, o script orienta a instalação das Xcode Command Line Tools. Ele não instala nada sozinho. Há instruções de compilação manual e uma nota sobre Windows no [guia do código](codigo/LEIA_ME.md).

## Quem pode fazer o quê

| Ação | Administrador | Gerente | Operador | Consulta |
|---|:---:|:---:|:---:|:---:|
| Consultar estoque e alertas | Sim | Sim | Sim | Sim |
| Trocar a própria senha | Sim | Sim | Sim | Sim |
| Registrar entradas e saídas | Sim | Sim | Sim | Não |
| Manter produtos | Sim | Sim | Não | Não |
| Gerar relatórios | Sim | Sim | Não | Sim |
| Fazer backup | Sim | Sim | Não | Não |
| Administrar usuários | Sim | Não | Não | Não |

O Operador não gera relatório. Essa diferença também aparece nos diagramas e na verificação de permissão do código.

## Organização das pastas

```text
PIM-II/
├── EstoqueFacil.command    # Dois cliques para abrir o programa no Mac
├── codigo/                 # Script e instruções de execução
│   └── src/                # Fonte C em camadas: apresentacao, negocio, seguranca, dados e comum
├── docs/                   # Explicações, verificação e checklist
│   └── trabalho/           # PIM em Word e capítulos para ler no GitHub
├── diagramas/
│   ├── imagens/            # Um PNG para cada diagrama
│   ├── fontes/             # Arquivos editáveis DOT, PlantUML e Mermaid
│   └── vetores/            # Versões SVG
└── gestao/
    └── reunioes/           # Dez registros semanais, separados por data
```

## Como as matérias aparecem

Pesquisa ajuda a entender o problema da papelaria. Engenharia de Software Ágil organiza requisitos, prioridades e semanas de trabalho. Banco de Dados aparece na modelagem dos registros, nos relacionamentos e nas regras de integridade, mesmo sem SQL. Programação em C coloca essas regras em funcionamento. Redes trata da estrutura da loja e das possibilidades de evolução. Direitos Humanos entra no cuidado com linguagem, inclusão, dados pessoais e permissões.

## Acompanhamento do grupo

O cronograma tem dez semanas, de **01/09/2026 a 09/11/2026**, com um encontro previsto a cada terça-feira. O último começa em 03/11. Chamamos esses encontros de “dailys semanais”, mas são uma adaptação: a Daily Scrum do guia acontece a cada dia de trabalho.

Os relatos anteriores foram reconstruídos como modelos e estão marcados com `[AJUSTAR]`, pois não temos atas para confirmar cada fala. Na organização feita em 12/10/2026, as semanas 7 a 10 ainda são futuras e aparecem como planejamento. A existência do código no repositório não comprova que uma reunião aconteceu. Os arquivos acompanham as mesmas histórias H01 a H10 do backlog.

## O que já foi verificado e o que falta

O programa foi compilado sem avisos e executado no macOS. Depois da organização em camadas, a versão nova repetiu as mesmas saídas da anterior nos roteiros de teste. Foram conferidos login, permissões, movimentações, alerta, relatório, backup e situações de falha. A [documentação de verificação](docs/VERIFICACAO.md) explica os testes e seus limites.

Windows e Linux ainda precisam de teste. Também faltam a avaliação com usuários e a medição de desempenho prevista nos requisitos. O foco desta versão é conseguir ler, entender e conferir o código.

A senha fica como um hash simples, usado para estudar a comparação de senhas. Isso não oferece a proteção adequada para um sistema real. O arquivo local não é criptografado, deve ser usado por uma instância de cada vez e não tem garantia de compatibilidade entre compiladores. Essas limitações estão descritas no trabalho.

## Integrantes e referências

Product Owner: Vinicius do Prado Fontes - RA: R331JF5. Scrum Master: João Pedro Silva Baranzeli - RA:H884977. Equipe de desenvolvimento: Roger Jesus Maia - RA:F36BEG7. 
Cidade: São José dos Campos, 2026.

As fontes consultadas estão nas [referências do PIM](docs/trabalho/referencias.md). Antes da entrega, precisamos preencher os dados do grupo, validar os registros semanais e revisar os campos indicados no [checklist](docs/CHECKLIST_WORD.md).
