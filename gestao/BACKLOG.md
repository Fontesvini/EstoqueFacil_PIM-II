# Backlog do EstoqueFácil

Backlog é a lista do trabalho que precisa ser organizado. As histórias abaixo descrevem quem precisa da função, o que deseja fazer e por quê. Pontos comparam o esforço entre tarefas; não são horas nem notas. 

Quadro 4 - Backlog do produto

| Item | História de usuário | Prior. | Pontos | Critério de aceite |
|---|---|---|---:|---|
| H01 | Como dono do MEI, quero entrar com minha conta, para proteger o acesso ao estoque. | Alta | 5 | Login correto abre o menu; terceiro erro bloqueia a conta. |
| H02 | Como dono do MEI, quero definir níveis, para limitar o que cada pessoa faz. | Alta | 5 | Administrador mantém contas; demais níveis têm a ação recusada. |
| H03 | Como usuário, quero trocar minha senha, para manter o controle da minha conta. | Média | 2 | Exige senha atual e confirmação da nova. |
| H04 | Como gerente, quero cadastrar produtos, para organizar o que a loja vende. | Alta | 3 | Código único; mínimo válido; saldo inicial zero. |
| H05 | Como operador, quero registrar entradas, para atualizar a quantidade recebida. | Alta | 3 | Saldo e histórico aumentam juntos. |
| H06 | Como operador, quero registrar saídas, para acompanhar a retirada de produtos. | Alta | 5 | Saída acima do saldo é recusada. |
| H07 | Como dono do MEI, quero ver alertas, para decidir o que repor. | Alta | 3 | Aparece abaixo e no mínimo; desaparece acima dele. |
| H08 | Como operador, quero buscar um produto, para informar sua disponibilidade. | Alta | 2 | Busca por código ou nome mostra o saldo correto. |
| H09 | Como contadora, quero consultar e exportar o histórico, para conferir movimentações. | Média | 3 | Filtros por produto e período funcionam; Operador não acessa. |
| H10 | Como dono do MEI, quero fazer backup, para recuperar os dados se houver perda. | Média | 3 | Cópia abre em teste; só Administrador e Gerente executam. |

Fonte: elaborado pelo grupo (2026). 

Pronto exige mensagem clara, permissão, teste principal e de falha, compilação sem avisos e documentação correspondente. Para texto, exige fonte e requisitos conferidos.

Pesquisa e modelagem também ocupam sprints. Trata-se de práticas do Scrum adaptadas ao PIM, sem afirmar que toda semana gerou software utilizável.


## Tarefas de apoio

Essas tarefas sustentam as histórias. Elas não criam requisitos funcionais adicionais.

| Código | Tarefa | Semana principal | Relação com as histórias |
|---|---|---|---|
| T01 | Caracterização da papelaria e pesquisa | 1 | Entender a necessidade de H04 a H08 |
| T02 | Requisitos, permissões e ordenação do backlog | 2 | Delimitar H01 a H10 |
| T03 | Casos de uso e demais diagramas de funcionamento | 3 | Conferir os fluxos de H01 a H10 |
| T04 | Modelagem e arquivo local | 4 | Guardar os dados necessários às histórias |
| T05 | Rede, privacidade e inclusão | 8, com cuidados desde o início | Apoiar acesso, uso e cópia dos dados |
| T06 | Integração e verificação do grupo | 9 | Conferir H01 a H10 e os cinco RNF |
| T07 | Revisão do Word, README e entrega | 10 | Manter a documentação de todas as histórias |

## Como conferir uma história

Relacionamos H01, H02 e H03 ao RF01; H04 ao RF02; H05 e H06 ao RF03; H07 ao RF04; H08, H09 e H10 ao RF05. Backup também atende ao RNF04.

O estado técnico do programa está em [Verificação](../docs/VERIFICACAO.md). Uma função presente no código não torna uma sprint automaticamente concluída: faltam a revisão do grupo e os testes previstos para aquela semana.

[Cronograma](CRONOGRAMA.md) | [Registros semanais](reunioes/README.md)
