/* TAD Dicionario */
typedef struct dicio TDicioEstatico;
typedef int (*TCompararDE)(void *elms, void *chave);

TDicioEstatico *criarDicioEstatico();
void inserirDE(TDicioEstatico *dicio, int idx, void *elm);
void *buscarSequencialDE(TDicioEstatico *dicio, void *chave, TCompararDE comparar);
void *buscarBinariaDE(TDicioEstatico *dicio, void *chave, TCompararDE comparar);
