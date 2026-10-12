# Conferência entre documento, diagramas e código

Código e diagramas conferidos em 11 de outubro de 2026: programa simples em um arquivo C, com armazenamento local em estoque_local.dat.

## Relações verificadas

| Diagrama | Correspondência no projeto |
| --- | --- |
| 01 Casos de uso | Usuário reúne ações comuns. Consulta e Operador herdam Usuário; Gerente herda ambos; Administrador herda Gerente. O Operador não recebe relatórios. |
| 02 Erros com extend | Os erros correspondem aos fluxos alternativos descritos: credenciais, inatividade, bloqueio, duplicidade, permissão, produto, quantidade, saldo e arquivo. A falha no banco da proposta inicial foi adaptada para falha no arquivo. |
| 03 Classes | Usuario, Produto, Movimentacao e Dados representam structs. Autenticacao, Estoque e ArquivoLocal são grupos didáticos de funções, sem afirmar que C implementa classes. |
| 04 Atividade do alerta | Percorre os produtos, exige ativo e compara quantidade <= minimo. Inclui o caso sem alertas. |
| 05 Fluxo do login | Até três rodadas na função; contador persistente por conta; terceiro erro de senha bloqueia por 300 segundos. Erro de gravação impede autenticação. |
| 06 Pacotes | Separa responsabilidades de interface, autenticação, regras e dados dentro do único arquivo C. Não representa arquivos de código separados. |
| 07 MER | Usuario e Produto se relacionam com Movimentacao. Categoria e fornecedor são atributos de Produto; nivel é atributo de Usuario. |
| 08 DER | Campos e tipos correspondem às structs reais. Cada movimentação guarda posições válidas de usuário e produto nos vetores. Inativar preserva posições e histórico. |
| 09 Rede | Endereços iguais ao quadro do PIM. Rede e VLANs são propostas. O celular não acessa o programa; nuvem é cópia externa manual, sem sincronização implementada. |

A conferência automática comparou 44 combinações de quatro níveis e onze ações entre a herança e associações do modelo de casos de uso e a função verificarPermissao. Todas coincidiram. O cadastro do diagrama agrupa a manutenção de produtos explicada em RF02 e no quadro de permissões.

O documento tem dez regras de negócio, cinco requisitos funcionais e cinco não funcionais. O código completo fica no arquivo C separado. As quatro figuras inseridas são as mesmas imagens correspondentes da pasta diagramas.

## Execução realmente verificada

Compilação com clang no macOS: -std=c11 -Wall -Wextra -Wpedantic -Werror, sem avisos. Testes em pastas isoladas, sem alterar registros pessoais.

Foram verificados: criação de demonstração e primeiro Administrador; login e senha incorreta; bloqueio persistente após três erros; troca da própria senha; permissões dos quatro níveis; login único; cadastro, alteração e inativação de usuários; produtos; entradas e saídas; quantidades inválidas; saída acima do estoque; alerta no valor mínimo; preservação de saldo e histórico; relatório TXT com filtros; backup; restauração; recusa de arquivo truncado; falha real de gravação; senha sem eco em terminal e restauração do terminal.

A expiração do bloqueio foi testada antecipando o prazo apenas nos dados de teste. O grupo pode repetir o teste aguardando os cinco minutos reais, conforme o roteiro. O script rodar.sh --demo também foi executado em uma pasta isolada: compilou, abriu, aceitou login e apresentou o alerta esperado.

Não foram executados: programa no Windows ou Linux; teste de usabilidade com quatro pessoas; avaliação com tecnologias assistivas; medição de desempenho prevista no RNF02. Esses itens não são apresentados como aprovados no PIM.

## Documentos e imagens

Na conferência anterior, as 53 páginas do PIM e as dez páginas do caderno foram renderizadas e revisadas visualmente. A revisão para impressão descrita abaixo substitui aquela versão do PIM. Os nove PNGs têm fundo branco e 2.400 pixels de largura. Os fontes e os SVGs acompanham as imagens.

Os PNGs foram gerados com o motor Graphviz por Viz.js. O script de geração passou pela verificação de sintaxe e pelo aviso de ferramentas ausentes. Sua geração completa com os comandos nativos Graphviz, PlantUML e Mermaid CLI não foi executada neste ambiente. As imagens entregues já estão prontas.


## Revisão para o GitHub em 12/10/2026

O código e os nove diagramas mantêm o conteúdo conferido anteriormente. A revisão ampliou o planejamento para dez semanas, organizou os capítulos em Markdown e criou um arquivo para cada encontro. A primeira montagem tinha 53 páginas físicas e 30 de desenvolvimento.

Na revisão para impressão, as oito etapas passaram a ser subseções de 2 Desenvolvimento. Foram retiradas as quebras obrigatórias entre etapas e o apêndice com o código completo. As quatro figuras principais foram preservadas; os dois primeiros diagramas foram aproximados de suas explicações. As referências às seções e o sumário foram atualizados. O sumário agora é um campo automático do Word, com resultados preenchidos.

A versão revisada tem 39 páginas físicas e 29 de desenvolvimento. A numeração fica visível a partir da introdução, que mostra 6 no canto superior direito. Todas as páginas foram conferidas na renderização, e os 54 títulos do sumário tiveram seus números comparados com as páginas correspondentes. Esta revisão altera a documentação; não representa uma nova rodada de testes do programa.

Os relatos semanais não são evidências de teste. Antes de trocar `[AJUSTAR]` por uma afirmação, o grupo precisa confirmar o que aconteceu. Os resultados técnicos acima descrevem execuções reais no ambiente macOS de preparação, não uma avaliação com os participantes do grupo.

O passo a passo e os resultados esperados de cada requisito estão no [guia de testes do código](../codigo/LEIA_ME.md). Os requisitos completos estão na [Etapa 3](trabalho/03-engenharia-agil.md).

## Como registrar a próxima execução

| Data real | Pessoa ou grupo | Sistema e compilador | Caso | Resultado esperado | Resultado observado | Situação |
|---|---|---|---|---|---|---|
| [AJUSTAR] | [NOME] | [PREENCHER] | Saída de 7 com saldo 6 | Recusar e manter saldo 6 | [REGISTRAR] | Pendente |

Registre uma linha por teste. Se o resultado for diferente, anote a mensagem exibida e os passos usados. Não coloque senha pessoal nem arquivo real de estoque no repositório.
