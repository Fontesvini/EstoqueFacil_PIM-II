# 2.4 MODELAGEM DE BANCO DE DADOS

## 2.4.1 Organização dos dados e modelo conceitual

O EstoqueFácil guarda os registros em estoque_local.dat. Banco de Dados orienta a organização desse conteúdo, enquanto C faz a leitura e a gravação. O arquivo é a base local do exercício, sem os recursos de um sistema gerenciador.

A organização lembra as prateleiras etiquetadas da papelaria. Entidade é algo sobre o qual guardamos informações; atributo é uma característica, como nome. O MER, modelo entidade-relacionamento, descreve Usuario, Produto e Movimentacao. Seu desenho em notação de Chen fica no caderno separado.

Cardinalidade indica quantos registros participam de uma relação. Um produto pode ter nenhuma ou várias movimentações; cada movimentação pertence a um produto. Um usuário pode registrar várias movimentações ou nenhuma; cada lançamento tem um responsável. São relações de um para muitos.

Categoria e fornecedor principal são textos de Produto. nivel é um atributo de Usuario, com valores de 1 a 4. Esses campos não têm cadastros separados. O nível define permissões; o vínculo com a movimentação registra quem fez o lançamento.

## 2.4.2 DER e identificação dos registros

O DER, diagrama entidade-relacionamento, mostra os campos e vínculos na Figura 2. Os nomes acompanham as structs de C. A posição no vetor identifica internamente cada registro e permanece após a inativação.

Figura 2 - Registros e relacionamentos do EstoqueFácil

![Figura 2](../../diagramas/imagens/08_der.png)

Fonte: elaborado pelo grupo (2026).

Por exemplo, CAN001 pode ocupar a posição 0 em produtos. Uma saída guarda 0 no campo produto; usuario aponta da mesma forma para a conta responsável. Esses números são referências, não quantidades. No menu de usuários, a posição aparece acrescida de um.

Login e código são únicos. O programa confere os vínculos entre vetores; não existe um gerenciador independente realizando essa validação.

## 2.4.3 Modelo lógico e dicionário de dados

O modelo lógico descreve os registros e suas relações. O dicionário explica cada campo, evitando confundir saldo com quantidade movimentada.

Quadro 9 - Campos dos registros do EstoqueFácil

| Registro | Campos | Significado |
|---|---|---|
| Usuario | nome, login, senhaHash | Identificação da conta e hash da senha. |
| Usuario | nivel, ativo, erros, bloqueadoAte | Permissão, situação, erros e fim do bloqueio. |
| Produto | codigo, nome, categoria, fornecedor | Identificação e descrição da mercadoria. |
| Produto | quantidade, minimo, ativo | Saldo, limite para alerta e situação. |
| Movimentacao | produto, usuario | Posições dos registros vinculados. |
| Movimentacao | quantidade, tipo, data | Unidades; E ou S; data e hora. |

Fonte: elaborado pelo grupo (2026).

Textos ficam em vetores de char. Quantidades, nível e situação são inteiros; ativo vale 1 ou 0. data usa ano-mês-dia hora:minuto:segundo. bloqueadoAte guarda o instante comparado com o relógio do computador.

Dados reúne os três vetores, versao e os contadores totalUsuarios, totalProdutos e totalMovimentacoes. Reserva 50 usuários, 1.000 produtos e 10.000 movimentações. São limites do exercício, sem estudo de capacidade da loja.

## 2.4.4 Normalização e redução de repetições

Normalização analisa os dados para evitar repetições e problemas de atualização. A primeira forma normal, 1FN, pede um valor por campo. A segunda, 2FN, evita dependência de parte de uma chave composta. A terceira, 3FN, retira dependências entre atributos que não são chaves (Watt; Eng, 2014). Chave é o identificador de cada registro.

Quadro 10 - Organização antes e depois da normalização

| Etapa | Exemplo na papelaria |
|---|---|
| Antes da 1FN | Uma ficha de CAN001 guarda “entrada de 20; saída de 3” no mesmo campo. |
| Depois da 1FN | Cada lançamento tem uma linha: identificador, produto, nome do produto, usuário, nome do usuário, tipo e quantidade. |
| Verificação da 2FN | O identificador da movimentação é simples. Não há dependência de parte da chave, mas os nomes ainda se repetem. |
| Depois da 3FN | Produto e usuário têm cadastros próprios. A movimentação guarda referências, tipo e quantidade; os nomes vêm dos vínculos. |

Fonte: elaborado pelo grupo (2026).

Essa separação orienta os vetores. Formas normais descrevem modelos relacionais, não o formato binário .dat. Categoria e fornecedor ainda podem se repetir com grafias diferentes. Cadastros próprios poderiam padronizá-los. O saldo também é calculável pelo histórico, exigindo atualização conjunta.

## 2.4.5 Armazenamento físico e operações

Armazenamento físico é a forma concreta de guardar os registros. Em memória, usamos Dados. Ao salvar, fwrite grava essa estrutura inteira em um arquivo temporário. O conteúdo é binário: bytes organizados conforme as structs, sem linhas destinadas à leitura humana. carregarDados usa fread e confere limites e referências antes de aceitar o arquivo.

Essas operações ficam isoladas na camada de acesso a dados, em dados/repositorio.c. A gravação termina antes de substituir o principal. Antes de uma alteração, iniciarAlteracao copia os dados em memória. Na falha, confirmarAlteracao recupera essa cópia. Isso evita confirmar saldo sem lançamento, mas não oferece todas as garantias de recuperação de um SGBD.

As operações usam funções e laços: buscarProduto localiza o código; estaComEstoqueBaixo compara quantidade e minimo. No relatório, as posições da movimentação permitem obter código do produto e nome do responsável. Os trechos reais são explicados na seção 2.5.

## 2.4.6 Integridade e recuperação

Integridade é manter os registros conforme as regras. Uma saída de sete canetas com saldo cinco é recusada. Também são conferidos identificadores repetidos, valores negativos, produto inativo e permissão.

A inativação mantém posições e histórico. Administrador e Gerente podem gerar backup_local.dat; relatorio.txt não o substitui. A recuperação deve ser testada com o programa fechado, copiando o backup para outra pasta com o nome estoque_local.dat.

Use uma execução por vez. O formato depende do compilador e não garante transporte entre Mac e Windows. O arquivo não é criptografado; as permissões do menu não impedem cópia externa. Essas limitações serão retomadas nas seções 2.6 e 2.7.

## 2.4.7 Possibilidade de uso de NoSQL

NoSQL reúne alternativas ao modelo relacional. Em um banco de documentos, registros podem ter campos diferentes, como explica MongoDB ([s. d.]). Um catálogo ampliado poderia guardar cor e espessura da caneta, mas folhas e capa do caderno.

Aqui, os campos são conhecidos e as structs atendem ao exercício sem outra instalação. O .dat não é um banco NoSQL: continua sendo um arquivo administrado por C. Um banco de documentos seria uma evolução a estudar.

Ligação com as outras matérias: a modelagem organiza os registros que C utiliza. Requisitos definem validações; Redes orienta cópias externas; Direitos Humanos limita a coleta de dados.

[Índice do trabalho](README.md) | [Referências](referencias.md)
