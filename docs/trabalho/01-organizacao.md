# 2 CARACTERIZAÇÃO DA ORGANIZAÇÃO

## 2.1 Identificação, missão e público-alvo

A Papelaria Ponto do Caderno é uma organização fictícia criada pelo grupo para este projeto. Trata-se de um pequeno comércio de materiais de papelaria, situado em um bairro de [CIDADE]. O negócio é conduzido por Marcos Almeida, microempreendedor individual, com o apoio de uma funcionária, Júlia Santos. Renata Lima presta serviço externo de contabilidade. Os nomes e as situações apresentados fazem parte do cenário acadêmico.

A missão da papelaria é oferecer materiais de uso diário com atendimento próximo dos moradores. Seus principais produtos são cadernos, canetas, lápis, borrachas, colas e envelopes. O público-alvo inclui estudantes, famílias e pequenos escritórios do bairro. A loja trabalha com produtos vendidos por unidade. Uma caixa comprada do fornecedor precisa ser convertida em unidades antes de entrar no estoque do sistema.

## 2.2 Estrutura organizacional e responsabilidades

Marcos cuida das compras e do atendimento, e Júlia auxilia no balcão e na reposição. A contadora não altera o estoque. A estrutura considera a orientação do Portal do Empreendedor sobre a contratação de um empregado pelo MEI (Brasil, [s. d.]).

Nível de acesso representa o que uma pessoa pode fazer no programa. Não exige um cargo diferente para cada nível.

Quadro 1 - Pessoas e níveis de acesso na organização fictícia

| Nível | Pessoa ou situação | Responsabilidade no EstoqueFácil |
|---|---|---|
| 1. Administrador | Marcos, proprietário | Acessar todas as funções e administrar usuários. |
| 2. Gerente | Júlia, se receber essa responsabilidade | Manter produtos, movimentar estoque, consultar relatórios e fazer backup. Não administrar usuários. |
| 3. Operador de estoque | Júlia na situação inicial | Registrar entradas e saídas, consultar estoque e visualizar alertas. Não gerar relatórios. |
| 4. Consulta | Renata, contadora externa | Consultar estoque, alertas e relatórios, sem alterar os registros do negócio. |

Fonte: elaborado pelo grupo (2026).

Júlia terá uma conta com um nível por vez. Uma mudança para Gerente será feita na mesma conta. Marcos já pode exercer a gestão com seu acesso de Administrador. As contas de demonstração serão apenas para teste.

## 2.3 Processos de negócio e problemas identificados

Processo de negócio é a sequência de atividades usada para atender uma necessidade da organização. Na papelaria, a reposição começa quando Marcos percebe que algum produto está acabando. Depois ele confere o que precisa comprar, recebe a mercadoria e guarda os itens. A venda faz o caminho contrário: o produto sai da prateleira e deveria ter sua quantidade descontada.

No cenário estudado, parte das compras é anotada em um caderno. As saídas nem sempre são registradas no momento do atendimento. Marcos também usa a memória para decidir o que comprar. Isso dificulta saber se uma caneta está disponível ou se já acabou. Júlia precisa procurar a mercadoria nos fundos antes de responder ao cliente.

Uma situação fictícia resume o problema: Marcos acreditava que ainda havia uma caixa de canetas azuis, mas as últimas unidades já tinham sido vendidas. Um cliente procurou esse produto e saiu sem comprar. O episódio ilustra o risco do controle incompleto; não é um resultado observado em uma pesquisa de campo.

## 2.4 Justificativa e limites da solução

O EstoqueFácil é um programa de terminal, isto é, uma aplicação usada por menus e mensagens de texto. O usuário escolherá opções numeradas, como consultar estoque ou registrar entrada. O programa foi escrito em C, e os registros ficam no arquivo local estoque_local.dat, no próprio computador.

A proposta concentra o trabalho em produtos, movimentações, alertas e relatórios. Não inclui emissão de notas fiscais, pagamento, cadastro de clientes ou venda pela internet. A saída de mercadoria representa uma redução do estoque, mas não substitui um sistema fiscal. Categoria e fornecedor principal são informações preenchidas no cadastro de cada produto.

Marcos e Júlia precisarão registrar as saídas e conferir o estoque físico. O programa não detecta sozinho uma venda. Não há medição de aumento de vendas ou de redução de perdas.

Ligação com as outras matérias: conhecer a papelaria ajuda a pesquisar o problema e a definir os requisitos. As responsabilidades de Marcos e Júlia orientam as permissões, os dados que serão guardados e as funções em C. O uso de um único computador também delimita a rede necessária.

[Índice do trabalho](README.md) | [Referências](referencias.md)
