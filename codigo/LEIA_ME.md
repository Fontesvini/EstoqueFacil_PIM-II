# EstoqueFácil simples, sem SQL

O código completo está na pasta **src**, organizado em camadas: apresentação, regras de negócio, autenticação e controle de acesso, e acesso a dados. Basta essa pasta e um compilador C. Não usa SQLite, comandos SQL, servidor, biblioteca externa ou instalação de banco de dados.

Os dados ficam em structs, que agrupam informações, e vetores, que guardam vários registros do mesmo tipo. O programa salva esses vetores em **estoque_local.dat**, criado automaticamente na pasta `codigo/src/output`. Cada alteração é gravada na hora. É um arquivo local de dados, sem um sistema gerenciador de banco.

## Como abrir o programa

Escolha um dos jeitos abaixo. Todos compilam o código, se precisar, e abrem o programa.

- **Dois cliques:** no Finder, dê dois cliques em `EstoqueFacil.command`, na pasta principal do projeto. Na primeira vez, se o Mac pedir confirmação, clique com o botão direito no arquivo e escolha **Abrir**.
- **VS Code:** abra a pasta do projeto (a principal, `codigo` ou `src`) e aperte **F5**, ou **Cmd+Shift+B**. O programa abre no Terminal do VS Code. O botão ▶ do Code Runner também funciona, com qualquer arquivo `.c` aberto.
- **Terminal:** na pasta `codigo`, execute `bash rodar.sh`.

Cada cadastro, movimentação ou alteração é gravado na hora em `codigo/src/output/estoque_local.dat`, que funciona como o banco de dados local. Na mesma pasta ficam o `relatorio.txt` e o `backup_local.dat`. O programa cria essa pasta sozinho e sempre usa a mesma, não importa de onde seja aberto. Ao abrir, o programa mostra o caminho completo onde está salvando. Para fechar, use a opção 0.

Na primeira vez, sem dados, o programa pede o cadastro do primeiro Administrador. Para testar com dados de exemplo, rode `bash rodar.sh --demo` numa pasta sem `estoque_local.dat`: são criados os logins `admin`, `gerente`, `operador` e `consulta`, todos com a senha `teste123`. A senha não aparece enquanto você digita.

Na demonstração, há uma conta de cada nível: `admin`, `gerente`, `operador` e `consulta`. Todas usam `teste123`, somente para teste. CAN001 começa com 3 unidades e mínimo 5; CAD001 começa com 10 e mínimo 3. Por isso, a caneta já aparece no alerta.

A demonstração só pode ser criada quando ainda não há dados. Para começar do zero, feche o programa e renomeie `src/output/estoque_local.dat` (por exemplo, para `estoque_antigo.dat`). Na próxima abertura, o programa pedirá o cadastro do primeiro Administrador.

### Se o Mac ainda não tiver o compilador

O script informa quando faltam as ferramentas de desenvolvimento. No terminal, execute:

```sh
xcode-select --install
```

Conclua a instalação apresentada pelo macOS, abra um terminal novo e repita `bash rodar.sh --demo`. O script não instala ferramentas automaticamente. Não é preciso Homebrew nem biblioteca de banco de dados.

As extensões C/C++ e CodeLLDB podem ajudar a editar e depurar no VS Code, mas não são necessárias para compilar pelo terminal. Use a pasta que contém o código como pasta de trabalho.

### Compilar sem o script

```sh
clang -std=c11 -Wall -Wextra -Wpedantic -Isrc src/main.c src/*/*.c -o estoquefacil
./estoquefacil --demo
```

`-Isrc` indica onde procurar os arquivos `.h`. Por isso, os `#include` usam o nome da camada, como `"dados/repositorio.h"`. `src/*/*.c` reúne os arquivos de todas as camadas.

### Nota para Windows

Com GCC já instalado, use no terminal da pasta:

```sh
gcc -std=c11 -Wall -Wextra -Isrc src/main.c src/comum/validacao.c src/dados/repositorio.c src/seguranca/acesso.c src/seguranca/contas.c src/negocio/produtos.c src/negocio/movimentacoes.c src/negocio/relatorios.c src/negocio/demo.c src/apresentacao/entrada.c src/apresentacao/mensagens.c src/apresentacao/menu.c src/apresentacao/telas_usuarios.c src/apresentacao/telas_estoque.c src/apresentacao/telas_relatorios.c -o estoquefacil.exe
.\estoquefacil.exe --demo
```

Os arquivos aparecem um a um porque o terminal do Windows nem sempre expande `*`. O código-fonte é o mesmo. O script `rodar.sh` foi preparado para Mac. A execução no Windows ainda precisa ser testada; não foi confirmada nesta entrega.

## Os cinco requisitos funcionais

| Requisito | O que o código faz |
|---|---|
| RF01 - Usuários e login | Cadastra usuários; valida senha; aplica quatro níveis; permite trocar a própria senha. Só Administrador altera nível e situação de outras contas. |
| RF02 - Produtos | Cadastra código, nome, categoria, fornecedor e mínimo; altera dados; inativa ou reativa sem apagar histórico. |
| RF03 - Movimentações | Registra entrada e saída, data, hora e usuário. Recusa quantidade inválida ou saída acima do saldo. |
| RF04 - Alerta | Percorre os produtos ativos e mostra `[ESTOQUE BAIXO]` quando saldo é menor ou igual ao mínimo. |
| RF05 - Consultas e relatório | Consulta estoque por código ou nome; gera estoque ou histórico em relatorio.txt, com filtros; faz backup local conforme a permissão. |

Administrador acessa tudo. Gerente mantém produtos, movimenta, gera relatórios e faz backup. Operador movimenta e consulta estoque, sem relatórios. Consulta apenas consulta estoque e gera relatórios. Todos podem trocar a própria senha.

## Os cinco requisitos não funcionais

| Requisito | Implementação e forma de verificar |
|---|---|
| RNF01 - Usabilidade | Menus numerados e mensagens diretas. O teste com quatro pessoas ainda deve ser feito pelo grupo: meta de três registrarem uma entrada sem ajuda em até três minutos. |
| RNF02 - Desempenho | Vetor com limite de mil produtos. A medição de dez consultas com meta de até dois segundos continua pendente. O foco do exercício é clareza, não otimização. |
| RNF03 - Segurança | Senha armazenada como hash didático; permissões; bloqueio por cinco minutos após três erros. Conferidos em testes de execução. |
| RNF04 - Confiabilidade | Grava em arquivo temporário antes de substituir o principal. Em erro de gravação, desfaz a alteração em memória. Backup e reabertura foram testados. |
| RNF05 - Compatibilidade | Código preparado para Mac e Windows, sem internet durante o uso. Compilado e executado no Mac. Windows e Linux pendentes de teste. |

O hash simples não é proteção adequada para senhas reais. Um sistema de produção precisa de uma função própria para senhas, como Argon2id ou bcrypt com salt. Essa é uma limitação do exercício, conforme as [orientações da OWASP](https://cheatsheetseries.owasp.org/cheatsheets/Password_Storage_Cheat_Sheet.html).

## Para entender o código

O programa segue uma arquitetura em camadas. Cada pasta de `src` é uma camada, e uma camada só chama as de baixo:

| Camada | Pasta | Responsabilidade |
|---|---|---|
| 1. Apresentação | `apresentacao/` | Menus, telas, leitura do teclado e mensagens. Não grava arquivos. |
| 2. Regras de negócio | `negocio/` | Produtos, saldo, movimentações, alerta, relatório, backup e demonstração. |
| 3. Autenticação e controle de acesso | `seguranca/` | Login, bloqueio, sessão, permissões, contas e hash da senha. |
| 4. Acesso a dados | `dados/` | Leitura e gravação de `estoque_local.dat`, cópia para desfazer e backup. |
| Comum | `comum/` | Structs, códigos de `Resultado` e validações usadas por todas. |

Comece por `main.c`. Ele carrega os dados, chama o login e mostra o menu. Depois leia `telaLogin` e `autenticar`, `verificarPermissao`, `telaManterProduto` e `salvarProduto`, `telaMovimentacao` e `registrarMovimentacao`. Os arquivos `.h` explicam o que cada módulo oferece.

As regras não escrevem na tela: devolvem um `Resultado`, e `mostrarResultado` exibe a mensagem. As regras também conferem a permissão, mesmo quando o menu já conferiu. Assim, nenhuma operação depende só da tela para ser protegida.

`fread` lê o arquivo local e `fwrite` grava os registros. `iniciarAlteracao` guarda uma cópia da situação anterior; `confirmarAlteracao` grava e, se a gravação falhar, desfaz a alteração. Categoria e fornecedor são textos dentro do produto, sem tabelas separadas.

O trecho de ocultação da senha usa funções do terminal e é mais específico. Ele ficou isolado em `lerSenha`, no arquivo `apresentacao/entrada.c`. No Mac usa termios; no Windows, conio.h aparece somente no bloco de Windows. A troca do arquivo no Windows usa [MoveFileExA](https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-movefileexa).

## Teste rápido

1. Entre como `operador`. Registre entrada de 3 em CAN001: saldo passa de 3 para 6 e sai do alerta.
2. Tente saída de 7: deve recusar. Retire 1: saldo 5, e o alerta volta.
3. Digite a opção 8, relatório: o Operador deve receber acesso negado.
4. Saia e entre como `consulta`. Gere um relatório e abra relatorio.txt.
5. Como `gerente`, use a opção 9 para backup. Como `admin`, use a opção 10 para usuários.

Foram testados no Mac: compilação sem avisos, login, bloqueio persistente, quatro níveis, cadastro e inativação, movimentações, alerta, relatório, backup, recuperação e falha real de gravação. O bloqueio expirado foi testado com prazo antecipado somente no arquivo de teste. O teste com pessoas ainda não foi realizado.

Na reorganização em camadas, em 28/10/2026, a versão nova e a anterior receberam os mesmos roteiros de entrada no Mac: login, bloqueio, quatro níveis, usuários, senha, produtos, movimentações, relatório, backup, demonstração e primeiro Administrador. As saídas foram idênticas. As structs não mudaram, então um `estoque_local.dat` da versão anterior continua sendo aberto.

## Limites desta versão

Use uma instância por vez. Há espaço para 50 usuários, mil produtos e dez mil movimentações. A pesquisa por nome diferencia maiúsculas de minúsculas. O relatório e o backup têm nomes fixos e pedem confirmação antes de substituir a cópia anterior.

O arquivo .dat é binário e depende do formato das structs do compilador. Use o backup no mesmo ambiente; não há garantia de transportar esse arquivo entre Mac e Windows. O código pode ser compilado nos dois sistemas, mas os dados não têm um formato de intercâmbio. O arquivo também não é criptografado.

Para recuperar, feche o programa, guarde uma cópia do `estoque_local.dat` atual e copie `backup_local.dat` com o nome `estoque_local.dat`, na pasta `codigo/src/output`.

## Conferência manual dos requisitos

Faça estes testes em uma pasta de demonstração. Use a opção 0 para sair e execute novamente para trocar de conta.

| Teste | Ação | Resultado esperado |
| --- | --- | --- |
| Login válido | Entrar com admin e teste123 | Abrir menu de nível 1. |
| Administrador | Opção 10, cadastrar uma conta | Pedir nome, login, nível e senha; recusar login repetido. |
| Gerente | Entrar como gerente; tentar opção 10 | Negar a ação; permitir produtos e backup. |
| Operador | Entrar como operador; tentar opção 8 | Negar relatório; permitir entradas e saídas. |
| Consulta | Entrar como consulta; tentar opção 3 | Negar entrada; permitir estoque e relatório. |
| Entrada | Com Operador, entrada de 3 em CAN001 | Saldo passa de 3 para 6 na demonstração ainda não alterada. |
| Saldo insuficiente | Tentar saída de 7 quando o saldo for 6 | Recusar e manter saldo 6. |
| Alerta no limite | Retirar 1 quando o saldo for 6 | Saldo 5 e aviso de estoque baixo. |
| Relatório | Com Consulta, opção 8, relatório de estoque | Gerar relatorio.txt após confirmação. |
| Backup | Com Gerente, opção 9 | Gerar backup_local.dat após confirmação. |
| Senha incorreta | Informar senha errada de uma conta ativa | Negar o acesso e registrar um erro. |
| Três erros | Repetir até completar três senhas erradas da mesma conta | Bloquear por cinco minutos, inclusive após reabrir. |
| Fim do bloqueio | Esperar cinco minutos desde o terceiro erro e tentar a senha correta | Liberar o acesso e zerar o contador. |

Registre o resultado observado, o sistema operacional e a data. A medição de desempenho e a avaliação com pessoas continuam pendentes. A expiração do bloqueio foi simulada na verificação técnica; o roteiro acima permite ao grupo testá-la também pelo tempo real.
