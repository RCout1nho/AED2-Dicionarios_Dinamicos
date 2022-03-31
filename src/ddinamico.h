typedef int (*TComparadorDD)(void *chave, void *carga);
typedef void (*TImpressoraDD)(void *carga);

/* TAD Dicionario */
typedef struct ddinamico TDicioDinamico;
TDicioDinamico *criarDicioDinamico(int fc, int tamanho, TComparadorDD comparador, TImpressoraDD impressor);
void inserirDD(TDicioDinamico *dicio, int chave, void *carga);
void *buscarDD(TDicioDinamico *dicio, int chave);
void *removerDD(TDicioDinamico *dicio, int chave);
void imprimirDD(TDicioDinamico *dicio);
