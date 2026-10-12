# 2.2 PESQUISA CIENTÍFICA E INOVAÇÃO

## 2.2.1 Problema e objetivos da pesquisa

A pergunta que orienta o projeto é: como um sistema local e simples pode ajudar uma pequena papelaria a acompanhar entradas e saídas e perceber a necessidade de reposição, respeitando as permissões de cada usuário?

O objetivo geral é desenvolver uma solução que organize o controle de estoque da Papelaria Ponto do Caderno. Para isso, o grupo pretende compreender a rotina proposta, comparar alternativas existentes, definir as regras do sistema e representar os dados. A implementação será avaliada por situações de uso, como uma entrada válida, uma saída acima do saldo e um produto exatamente no estoque mínimo.

A pesquisa também considera a linguagem dos menus. Para Júlia, a mensagem deve indicar qual produto foi alterado e qual é o novo saldo.

## 2.2.2 Metodologia

A pesquisa é aplicada porque busca construir uma solução para um problema definido. É exploratória porque procura compreender esse problema e delimitar uma proposta inicial. É bibliográfica porque utiliza publicações e documentação técnica para apoiar as decisões. Essas classificações seguem a discussão de Prodanov e Freitas (2013) sobre natureza, objetivos e procedimentos de pesquisa.

O grupo descreve o negócio, relaciona problemas e funções e consulta fontes sobre desenvolvimento e banco de dados. As páginas dos sistemas existentes permitem identificar recursos, mas sua divulgação comercial não comprova resultados para a papelaria.

O documento preliminar do grupo, intitulado PIM 2 - EstoqueFácil ([NOMES DO GRUPO], [s. d.]), forneceu uma base de regras, requisitos e oito sprints. Sprint é um período curto de trabalho com um objetivo definido. Esse material foi adaptado para incluir usuários e permissões e para manter cinco requisitos funcionais. A versão atual amplia o planejamento para dez semanas. O material preliminar não contém atas de reuniões ou resultados de testes.

A tarefa de conversar com MEIs aparece no planejamento preliminar, mas não comprova entrevistas realizadas. Relatos de campo só serão acrescentados se o grupo os confirmar.

## 2.2.3 Fundamentação e trabalhos relacionados

Antes de abrir uma pequena loja, o dono precisa conhecer o bairro e o que as pessoas procuram. A pesquisa cumpre um papel parecido neste projeto: ajuda a entender o problema antes de escolher os menus e os dados que serão guardados. A decisão de usar C já faz parte do escopo acadêmico. A pesquisa orienta como usar essa linguagem em uma solução que o grupo consiga explicar.

A matéria de Banco de Dados ajuda a decidir quais informações guardar e como relacioná-las. Na versão do EstoqueFácil, produtos, usuários e movimentações ficam em estruturas da linguagem C e são salvos em um arquivo local. Um sistema gerenciador de banco de dados, ou SGBD, é um software que administra o armazenamento e o acesso aos registros. O protótipo usa leitura e gravação de arquivo pelo próprio programa, sem um SGBD.

O grupo examinou recursos descritos em três alternativas reais. A comparação não pretende afirmar que o EstoqueFácil é superior a produtos disponíveis no mercado. Ela ajuda a selecionar um conjunto pequeno de funções para a papelaria.

Quadro 2 - Alternativas relacionadas ao controle de estoque

| Alternativa | Recurso documentado | Relação com a proposta |
|---|---|---|
| Planilha no LibreOffice Calc | Formatação condicional, que altera a aparência de células conforme uma condição | Uma planilha pode destacar saldos baixos. No EstoqueFácil, a comparação com o mínimo fica pronta no programa. |
| Bling | Controle de movimentações, relatórios e alertas de estoque mínimo | Confirma que acompanhar saldo e reposição é uma função já presente em sistemas de gestão. O projeto acadêmico tem um escopo menor. |
| Odoo Inventory | Regras de reposição baseadas em quantidades mínima e máxima | Mostra uma solução com planejamento de reposição mais amplo. O EstoqueFácil apenas exibe o alerta e não cria compras automaticamente. |

Fonte: elaborado pelo grupo (2026), com base em The Document Foundation ([s. d.]), Bling ([s. d.]) e Odoo ([s. d.]).

Uma planilha poderia atender parte da necessidade, mas alguém precisaria preparar as fórmulas e a organização dos lançamentos. O sistema proposto deixa esse caminho definido: escolher o produto, informar a quantidade e confirmar a operação. A vantagem esperada está nessa adaptação à rotina da loja, e não na criação de um recurso inexistente.

## 2.2.4 Justificativa científica e diferenciais

A justificativa do projeto é propor uma solução verificável para o controle incompleto. O grupo testará registros, saldos e alertas com resultados esperados definidos antes da execução.

Por exemplo, se uma caneta tem cinco unidades e mínimo de cinco, o alerta deve aparecer. Se uma entrada aumentar a quantidade para seis, o produto deve deixar a lista de reposição. Se uma saída de sete unidades for tentada nesse momento, o programa deve recusá-la. Os valores são exemplos de teste, não dados de uma empresa real.

A inovação proposta é uma mudança na forma de trabalhar da organização fictícia. O caderno dá lugar a registros organizados e consultáveis. O alerta de estoque baixo já existe em outros sistemas, como mostra a comparação anterior. O grupo não o apresenta como invenção. A contribuição está em reunir esse recurso com menus simples, funcionamento local e controle de acesso em um projeto compreensível para alunos do segundo semestre.

O funcionamento sem internet se aplica às tarefas do estoque. Já enviar uma cópia do arquivo de dados para um serviço de nuvem depende de conexão e será discutido como procedimento de backup. Backup é uma cópia de segurança que permite recuperar dados se o arquivo principal for perdido. Não será prometida sincronização automática entre computadores.

## 2.2.5 Forma de avaliação e limites da pesquisa

A avaliação terá testes funcionais, que conferem as regras, e observação de uso, que procura dificuldades com os menus. Cada tarefa indicará produto e quantidade.

O documento preliminar propõe observar quatro pessoas sem conhecimento técnico. Essa quantidade será usada apenas como um pequeno teste de sala de aula. Mesmo que três pessoas consigam registrar uma entrada sem ajuda, isso não permite afirmar que todos os MEIs terão a mesma facilidade. Os resultados só serão registrados depois de uma aplicação real do roteiro.

O desempenho terá prioridade baixa. O grupo dará preferência a nomes claros, funções curtas e saldo correto. Só haverá otimização se um teste mostrar necessidade.

Ligação com as outras matérias: a pesquisa transforma o problema da loja em decisões justificadas. Os testes ajudam a Engenharia de Software Ágil a definir o que está pronto. A organização dos registros orienta a modelagem e o código em C, enquanto o uso local e a coleta mínima de dados se ligam a Redes e Direitos Humanos.

[Índice do trabalho](README.md) | [Referências](referencias.md)
