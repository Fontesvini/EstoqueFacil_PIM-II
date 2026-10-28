# Conferência entre documento, diagramas e código

Código e diagramas conferidos em 11 de outubro de 2026 e revisados em 28 de outubro de 2026, quando o programa foi organizado em camadas na pasta codigo/src. O armazenamento continua local, em estoque_local.dat.

## Relações verificadas

| Diagrama | Correspondência no projeto |
| --- | --- |
| 01 Casos de uso | Usuário reúne ações comuns. Consulta e Operador herdam Usuário; Gerente herda ambos; Administrador herda Gerente. O Operador não recebe relatórios. |
| 02 Erros com extend | Os erros correspondem aos fluxos alternativos descritos: credenciais, inatividade, bloqueio, duplicidade, permissão, produto, quantidade, saldo e arquivo. A falha no banco da proposta inicial foi adaptada para falha no arquivo. |
| 03 Classes | Usuario, Produto, Movimentacao e Dados representam structs de comum/modelo.h. Autenticacao, Estoque e ArquivoLocal correspondem aos módulos seguranca, negocio e dados, sem afirmar que C implementa classes. |
| 04 Atividade do alerta | Percorre os produtos, exige ativo e compara quantidade <= minimo. Inclui o caso sem alertas. |
| 05 Fluxo do login | Até três rodadas na função; contador persistente por conta; terceiro erro de senha bloqueia por 300 segundos. Erro de gravação impede autenticação. |
| 06 Pacotes | Cada pacote é uma pasta de codigo/src: apresentacao, seguranca, negocio e dados. As setas do diagrama são as mesmas dependências dos #include. Não representa servidores separados. |
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

## Organização em camadas em 28/10/2026

O arquivo único estoquefacil.c foi dividido em módulos dentro de codigo/src, sem mudar o que o programa faz:

| Camada | Módulos | Pode usar |
|---|---|---|
| Apresentação | menu, telas_usuarios, telas_estoque, telas_relatorios, entrada, mensagens | Negócio, segurança e comum |
| Regras de negócio | produtos, movimentacoes, relatorios, demo | Segurança, dados e comum |
| Autenticação e controle de acesso | acesso, contas | Dados e comum |
| Acesso a dados | repositorio | Comum |

As regras devolvem um Resultado em vez de escrever na tela. As permissões são conferidas pelo menu e novamente pelas regras. As structs não mudaram; arquivos estoque_local.dat da versão anterior continuam aceitos.

Verificação realizada no macOS: compilação com -std=c11 -Wall -Wextra -Wpedantic -Werror, sem avisos. A versão anterior e a nova receberam os mesmos roteiros de entrada, cobrindo demonstração, primeiro Administrador, login, senha incorreta, bloqueio, usuário inativo, quatro níveis, cadastro e alteração de usuários, troca de senha, produtos, entradas, saídas, estoque insuficiente, produto inativo, relatório com período, backup e argumentos inválidos. As saídas e o relatório gerado foram idênticos. Um arquivo de dados criado pela versão anterior foi aberto pela nova.

As fontes dos diagramas 03 e 06 foram atualizadas para os nomes dos módulos. Os SVGs foram regenerados com Viz.js (motor neato, como os demais) e os PNGs foram convertidos com resvg, mantendo 2.400 pixels de largura e as mesmas alturas anteriores; as duas imagens foram conferidas visualmente. O Word e o caderno ainda precisam receber as seções e figuras revisadas, conforme o [checklist](CHECKLIST_WORD.md).

## Local fixo dos dados em 28/10/2026

Antes, o programa gravava `estoque_local.dat` na pasta de onde era aberto. Abrindo por caminhos diferentes, como o terminal, o botão do VS Code ou outra pasta, ele criava arquivos separados e parecia perder os cadastros. Agora `usarPastaDoPrograma`, em `dados/repositorio.c`, faz o programa usar sempre a pasta `codigo/output`, ao lado do executável, para o banco, o backup e o relatório. A pasta é criada se não existir, e a tela inicial mostra o caminho do arquivo.

Teste realizado no macOS: o programa foi aberto a partir de uma pasta diferente; o cadastro do Administrador, de um produto e de uma entrada de 20 unidades ficou gravado em `codigo/output/estoque_local.dat`, sem criar arquivo na outra pasta. Ao abrir de novo a partir de `/`, o login funcionou e a consulta mostrou o saldo 20.

Também foram incluídos `EstoqueFacil.command`, para abrir com dois cliques no Finder, e as configurações do VS Code para F5, Cmd+Shift+B e o botão do Code Runner.

O relatório e o backup gerados no mesmo teste também ficaram em `codigo/output`.

O passo a passo e os resultados esperados de cada requisito estão no [guia de testes do código](../codigo/LEIA_ME.md). Os requisitos completos estão na [Etapa 3](trabalho/03-engenharia-agil.md).

## Como registrar a próxima execução

| Data real | Pessoa ou grupo | Sistema e compilador | Caso | Resultado esperado | Resultado observado | Situação |
|---|---|---|---|---|---|---|
| [AJUSTAR] | [NOME] | [PREENCHER] | Saída de 7 com saldo 6 | Recusar e manter saldo 6 | [REGISTRAR] | Pendente |

Registre uma linha por teste. Se o resultado for diferente, anote a mensagem exibida e os passos usados. Não coloque senha pessoal nem arquivo real de estoque no repositório.
