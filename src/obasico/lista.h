typedef struct lista TListaSE;

typedef int (* TCompararLSE)(void*, void*);
typedef void (* TImpressoraLSE)(void*);

TListaSE* criarLSE();
void inserirInicioLSE(TListaSE* lse, void* carga);
void inserirFinalLSE(TListaSE* lse, void* carga);
void imprimirLSE(TListaSE *l, TImpressoraLSE impressora);
int tamanhoLSE(TListaSE *l);
void* acessarLSE(TListaSE *l, int pos);
void *removerInicioLSE(TListaSE *l);
void *removerFinalLSE(TListaSE *l);
void *removerPosicaoLSE(TListaSE *l, int pos);
void *buscarConteudoLSE(TListaSE* l, TCompararLSE comparador, void *chaveBusca);
void *removerConteudoLSE(TListaSE* l, TCompararLSE comparador, void *chaveBusca);
void inserirConteudoLSE(TListaSE *l, void *carga, TCompararLSE comparador);
//void inserirConteudoLSE(TListaSE *l, void *carga);
void destruirLSE(TListaSE* *rl);
