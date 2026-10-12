# 3 CONCLUSÃO

O EstoqueFácil atende ao objetivo de construir um protótipo compreensível para o controle de estoque da Papelaria Ponto do Caderno. A versão reúne login, níveis de acesso, cadastro de produtos, entradas e saídas, alerta de estoque baixo, relatório e backup. O uso de um arquivo local permite executar o exercício sem instalar um gerenciador de banco de dados.

A modelagem ajudou a separar usuários, produtos e movimentações. O vínculo entre os registros identifica a mercadoria e a conta responsável por cada lançamento. No código, as verificações impedem quantidade negativa e saída acima do saldo. O alerta inclui o valor igual ao mínimo, situação que poderia passar despercebida em uma conferência apenas visual da prateleira.

A verificação técnica em macOS conferiu os principais fluxos e situações de falha. Esses resultados demonstram o comportamento testado, mas não comprovam melhoria de vendas nem facilidade para todos os usuários. A observação com pessoas sem conhecimento técnico, a avaliação de acessibilidade e a execução no Windows continuam pendentes. Os integrantes devem registrar os próprios testes e ajustar os modelos de acompanhamento das sprints.

Há limites que precisam ser considerados antes de um uso real. O hash de senha é didático, o arquivo não é criptografado, há quantidades máximas de registros e apenas uma execução deve acessar os dados por vez. Uma evolução pode melhorar a proteção das senhas, a recuperação de cópias e o transporte dos dados. O uso por várias lojas exigiria rever a arquitetura e as gravações simultâneas.

Ligação com as outras matérias: o projeto mostrou como pesquisa, planejamento, dados, programação, rede e respeito às pessoas orientam decisões de um mesmo sistema. Para a papelaria, o benefício esperado depende tanto do programa quanto do cuidado de registrar e conferir as mercadorias.


[Índice do trabalho](README.md)
