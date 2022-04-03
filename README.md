# Como executar?

Abra o console nesse diretório e execute o seguinto comando:

```bash
gcc **/*.c -o program -lm && ./program < arquivo_entrada > arquivo_saida
```

Onde `arquivo_entrada` corresponde ao nome do arquivo de entrada, e `arquivo_saida` corresponde ao nome do arquivo de saida

Exemplo:

```bash
gcc **/*.c -o program -lm && ./program < bases-dicionario/Aventuras.base > out/saida1
```
