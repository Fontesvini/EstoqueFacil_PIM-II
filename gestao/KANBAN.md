# Quadro Kanban

Kanban mostra por onde cada tarefa está passando. Scrum organiza o ritmo das semanas, e o quadro ajuda a acompanhar o trabalho entre os encontros.

| Coluna | O que significa |
|---|---|
| Backlog | Trabalho identificado, ainda não escolhido para começar |
| A fazer | Tarefa escolhida para a semana |
| Fazendo | Tarefa em andamento, com limite de dois cartões no grupo |
| Revisão e Teste | Outra pessoa confere texto, código e resultado |
| Feito | Critérios de aceite e definição de pronto atendidos |

## Exemplo de uso na semana 5

O quadro é ilustrativo `[AJUSTAR]`. Não é um registro confirmado da situação naquela data.

| Backlog | A fazer | Fazendo, limite 2 | Revisão e Teste | Feito |
|---|---|---|---|---|
| H07: conferir alerta | H04: cadastrar produto | H01: validar login | H02: conferir permissões | T04: descrever structs |
| H09: exportar histórico | Preparar contas de teste | H03: trocar senha | Recusar login duplicado | T04: revisar DER |

Cada cartão precisa de código do backlog, tarefa, responsável `[NOME]` e uma forma de conferir o resultado. Se houver impedimento, anotar o motivo e o próximo passo. Um cartão não chega a Feito apenas porque o código compilou.

Este quadro fica em Markdown para ser fácil de editar. Ele não cria nem sincroniza um GitHub Project automaticamente.

[Backlog](BACKLOG.md) | [Cronograma](CRONOGRAMA.md)
