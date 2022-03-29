#include "stdio.h"
#include "stdlib.h"
#include "lista.h"

typedef struct elementoLSE{
  void *cargautil;
  struct elementoLSE* proximo; // sucessor
} TElementoLSE;

struct lista{
  struct elementoLSE* inicio;
  int tamanho;
};

typedef void (* TImpressoraLSE)(void*);
typedef int (* TCompararLSE)(void*, void*);

TListaSE* criarLSE(){
  TListaSE* lista = malloc(sizeof(TListaSE));

  lista->inicio = NULL;
  lista->tamanho = 0;

  return lista;
}

TElementoLSE* criarElementoLSE(void *carga){
  TElementoLSE* elem = malloc(sizeof(TElementoLSE));

  elem->cargautil = carga;
  elem->proximo=NULL;

  return elem;
}

void imprimirElementoLSE(TElementoLSE* e){

  //printf("%lf %lf %d %d\n", e->lat, e->lgi, e->data, e->hora);
  printf("%p", e->cargautil);
}

void inserirInicioLSE(TListaSE* lse, void* carga){
  TElementoLSE *novo = criarElementoLSE(carga);

  lse->tamanho++;
  if (lse->inicio == NULL){
    lse->inicio = novo;
  }else{
    novo->proximo = lse->inicio;
    lse->inicio = novo;
  }
}

void inserirFinalLSE(TListaSE* lse, void* carga){
  TElementoLSE *novo = criarElementoLSE(carga);

  lse->tamanho++;
  if (lse->inicio == NULL){
    lse->inicio = novo;
  }else{
    TElementoLSE *cam = lse->inicio;
    while (cam->proximo!=NULL){
      cam = cam->proximo;
    }
    cam->proximo = novo;
  }
}

void imprimirLSE(TListaSE *l, TImpressoraLSE impressora){

  TElementoLSE* cam = l->inicio;
  printf("inicio da impressão\n");
  while(cam!=NULL){
    printf("%p ", cam);
    impressora(cam->cargautil);
    cam = cam->proximo;
  }
  printf("\n");
}

int tamanhoLSE(TListaSE *l){
  return l->tamanho;
}

void* acessarLSE(TListaSE *l, int pos){
  TElementoLSE *cam = NULL;
  void *carga=NULL;
  if( (pos>=1) && (pos<=l->tamanho) ){
    int i=1;
    cam = l->inicio;
    while(i<pos){
      i++;
      cam = cam->proximo;
    }
    carga =   cam->cargautil;
  }
  return carga;
}

void *removerInicioLSE(TListaSE *l){
  TElementoLSE* elem;
  if (l->inicio == NULL){
    elem = NULL;
  }else{
    elem = l->inicio;
    l->inicio = elem->proximo;
    l->tamanho--;
  }
  void *carga = elem->cargautil;
  free(elem);
  return carga;
}

void *removerFinalLSE(TListaSE *l){
  return removerPosicaoLSE(l, l->tamanho);
}

void *removerPosicaoLSE(TListaSE *l, int pos){
    TElementoLSE* removido=NULL;

    TElementoLSE *ant=NULL;
    if( (pos<1) || (pos>l->tamanho) ){
      printf("ERRO: Fora do escopo\n");
      return removido;
    }
    TElementoLSE *cam=l->inicio;
    int i=1;
    while(i<pos){
      ant = cam;
      cam = cam->proximo;
      i++;
    }
    if (ant==NULL){
      removido = l->inicio;
      // atualizando a descrição da lista (o novo inicio)
      l->inicio = removido->proximo;
    }else{
      removido = cam;
      ant->proximo = removido->proximo;
    }
    // atualizando a descrição da lista (reduzir o tamanho)
    l->tamanho--;

    void *carga = removido->cargautil;
    free(removido);
    return carga;
}

void *buscarConteudoLSE(TListaSE* l, TCompararLSE comparador, void *chaveBusca){
  TElementoLSE *cam = l->inicio;

  while ((cam!=NULL) && ( comparador(cam->cargautil, chaveBusca) !=0 ) ){
    cam = cam->proximo;
  }
  if (cam == NULL)
    return NULL;
  else
    return cam->cargautil;
}

void *removerConteudoLSE(TListaSE* l, TCompararLSE comparador, void *chaveBusca){
  TElementoLSE *cam = l->inicio;
  TElementoLSE *ant=NULL;
  while ((cam!=NULL) && (comparador(cam->cargautil, chaveBusca)!=0) ){
    ant = cam;
    cam = cam->proximo;
  }

  if (cam!=NULL){  // encontrou o elemento
    if(l->inicio == cam){ // removendo o primeiro da lista (no inicio)
      l->inicio = cam->proximo;
      l->tamanho--;
    }else{ // removendo um intermediario
      ant->proximo = cam->proximo;
      l->tamanho--;
    }
    void *carga = cam->cargautil;
    free(cam);
    return carga;
  }else{
    return NULL;
  }
}


void destruirLSE(TListaSE* *rl){

  while((*rl)->inicio != NULL){
      void *carga = removerInicioLSE(*rl);
      free(carga);
  }
  free(*rl);
  *rl = NULL;
}

void inserirConteudoLSE(TListaSE *l, void *carga, TCompararLSE comparador){
  TElementoLSE* cam = l->inicio;
  TElementoLSE* ant=NULL;
  TElementoLSE *novo = criarElementoLSE(carga);

  while( (cam != NULL) && comparador(cam->cargautil, carga ) <= 0 ){
    ant = cam;
    cam = cam->proximo;
  }

  if (l->inicio == cam){ //
    l->inicio = novo;
    novo->proximo = cam;
    l->tamanho++;
  } else{
    novo->proximo = cam;
    ant->proximo = novo;
    l->tamanho++;
  }

}
