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
gcc **/*.c -o program -lm && ./program < bases-dicionario/Aventuras.base > saida.txt
```

## Observações

- A verificação se uma palavra é stopword foi feita (dicionário estático implementado em vetor e com busca binária)
- Arquivo de stopwords foi ordenado
- Função `df` para computar "frequência do termo" foi implementada

