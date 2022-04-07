# Documentação

## Equipe

- Gabriel de Oliveira Maciel
- José Ricardo Sampaio Coutinho II
- Juliana Pereira de Magalhães

## Como executar?

Abra o console nesse diretório e execute o seguinto comando:

```bash
gcc **/*.c -o program -lm && ./program < arquivo_entrada > arquivo_saida
```

Onde `arquivo_entrada` corresponde ao nome do arquivo de entrada, e `arquivo_saida` corresponde ao nome do arquivo de saida

Exemplo:

```bash
gcc **/*.c -o program -lm && ./program.o < bases-dicionario/Aventuras.base > saida.out
```

## Sobre as implementações

### **Determinar se uma palavra é `stopword`**

- Implementamos um `dicionário estático` responsável por conter as `stopwords`. O `dicionário estático` foi escolhido por motivos simples:
  
  1. As `stopwords` só serão lidas uma vez e não serão modificadas, ou seja, não há necessidade de operações de `inserção` ou `remoção` otimizadas.
  
  2. A busca será constantemente demandada, ou seja, precisamo de uma estrutura de dados rápida para consulta, e um `dicionário dinâmico` é o ideal para isso, uma vez que a busca no mesmo está implementada com um algoritmos de `busca binária`, contando com um tempo `O(log(n))`, bem melhor que uma `busca sequencial` quem tem tempo `O(n)`.

- O arquivo `stopwords_br_sorted.txt` foi preveamente ordenado, como sugere a descrição do trabalho.

### **Como as palavras são salvas**

- Implementamos um `dicionário dinâmico` responsável por conter todas as palavras que **não são stopwords**. No dicionário, cada palavra é uma `TAD TPalavra`, que associa: palavra, chave, e páginas onde a palavra ocorre.

### **Como uma palavra é buscada**

- Como o trabalho pede para que exibamos apenas as 5 ocorrências mais importante de uma palavra, implementamos a função `buscarPalavra` que é responsável por buscar se a palavra existe no dicionário e imprimir as 5 ocorrências mais significativas.

- Para calcular quais são as 5 ocorrências mais significativas, implementamos a solução da seguinte maneira:

  1. Verificar se a palavra existe no dicionário
  
  2. Se a palavra existe, precisamos ordenar as ocorrências pela ordem de importância, para isso:

      - Calculamos a o `tf-idf` da palavra em cada uma das páginas em que ela aparece e salvamos esse valor juntamente com os dados da ocorrência em um `vetor dinâmico`. (Optamos por um vetor dinâmico por que seria mais fácil para ordenar do que uma lista encadeada).

      - Depois de termos terminado de preencher o VD, o ordenamos (usando `quickSort`) pela ordem de importância
  
  3. Agora que temos o vetor ordenado, mostramos apenas até no máximo as 5 ocorrências (em ordem decrescente de importância)

## Sobre as análises

As análises ainda não foram realizadas
