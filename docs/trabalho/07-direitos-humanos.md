# 8 DIREITOS HUMANOS INCLUSÃO E SEGURANÇA

## 8.1 Acessibilidade e inclusão digital

A Declaração Universal dos Direitos Humanos reconhece a igualdade em dignidade e direitos e a proteção da vida privada (Organização das Nações Unidas, 1948). Na papelaria, uma dificuldade de Marcos com o computador deve orientar uma explicação melhor, sem constrangimento.

Inclusão digital envolve ter condições de usar a tecnologia para uma necessidade. O menu propõe tarefas curtas, como escolher a caneta e informar quantas chegaram. Uma apresentação pode usar os dados de demonstração antes dos registros do negócio. Não há pesquisa concluída comprovando facilidade para todos os MEIs.

Acessibilidade procura reduzir barreiras. A Lei Brasileira de Inclusão trata do acesso a informações, comunicação e tecnologias em condições de igualdade (Brasil, 2015). O EstoqueFácil oferece teclado, menus numerados e alerta escrito ESTOQUE BAIXO, sem depender de cor. Saldo e mínimo aparecem juntos.

As WCAG 2.2, diretrizes para conteúdo da Web, ajudam a pensar teclado, contraste e identificação de erros; não certificam este terminal (World Wide Web Consortium, 2024). Fonte e contraste dependem de sua configuração. Leitores de tela, ampliação e navegação ainda precisam ser experimentados com usuários.

Movimentações e inativações pedem confirmação. Não existe exclusão definitiva de produtos e usuários pelo menu, preservando a identificação do histórico. A pessoa pode conferir o saldo antes de confirmar uma saída.

Algumas mensagens evitam acentos por compatibilidade entre terminais. Essa escolha e textos técnicos, como o limite de um inteiro, precisam ser revistos na interface final. [AJUSTAR: registrar dificuldades observadas pelo grupo em testes reais.]

## 8.2 Privilégios de cada pessoa

Menor privilégio significa dar apenas o acesso necessário à tarefa. É como entregar a Júlia a chave do estoque, sem a dos cadastros de usuários. Ela começa como Operador e pode passar a Gerente se assumir produtos e backups. Renata usa Consulta, sem alterar estoque.

## 8.3 Privacidade e dados pessoais

A Lei Geral de Proteção de Dados Pessoais, LGPD, inclui finalidade, necessidade e segurança, que orientam a coleta mínima e sua explicação às pessoas (Brasil, 2018). Para a conta, são informados nome, login e senha. Não se pedem CPF, endereço ou dados de clientes.

O arquivo também guarda nível, situação, erros, bloqueio e responsável pelas movimentações. São informações de controle, além de nome, login e hash da senha. Campos livres devem evitar dados pessoais desnecessários.

Inativar mantém os registros. O uso real exigiria base legal, prazo de conservação e atendimento aos direitos dos titulares. Inativação não resolve pedidos de exclusão nem demonstra conformidade integral com a LGPD (Brasil, 2018).

## 8.4 Proteção das informações e limites do protótipo

calcularHash usa DJB2: começa em 5381, multiplica por 33 e soma o valor de cada caractere. O resultado hexadecimal pode coincidir para senhas diferentes. Não há salt, valor aleatório por senha usado em cálculos adequados de proteção.

O cálculo evita texto puro, mas é fraco para contas reais. A OWASP ([s. d.]) orienta Argon2id com salt; bcrypt é alternativa para sistemas legados. A melhoria exige outra implementação. O bloqueio também não impede tentativas sobre uma cópia roubada do arquivo.

Confiabilidade exige conferir gravações e backups. Confidencialidade restringe acesso ao conteúdo. O .dat não é criptografado e pode ser alterado fora do menu. O histórico identifica a conta, sem provar quem estava no teclado.

## 8.5 Impactos sociais e ética

Espera-se apoiar Marcos e Júlia na reposição. Isso depende de registrar e conferir a mercadoria. Uma divergência deve ser investigada antes de atribuir culpa. A implantação deve incluir orientação e espaço para dúvidas.

Ligação com as outras matérias: respeito orienta permissões, dados e mensagens. Pesquisa e sprints ajudam a identificar barreiras; rede e backups também exigem cuidado com as pessoas.

[Índice do trabalho](README.md) | [Referências](referencias.md)
