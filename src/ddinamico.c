#include "stdio.h"
#include "stdlib.h"
#include "./obasico/vdinamico.h"
#include "./obasico/lista.h"
#include "ddinamico.h"

/*
 * TAD Dicionario
 */
struct ddinamico
{
  /* Vetor dinamico */
  TVetorDinamico *hash;

  /* Fator de carga */
  int fc;

  /* Tamanho */
  int tamanho;

  /* Callback de comparacao */
  TComparadorDD comparador;

  /* Callback de impressao */
  TImpressoraDD impressor;
};

TDicioDinamico *criarDicioDinamico(int fc, int tamanho, TComparadorDD comparador, TImpressoraDD impressor)
{
  TDicioDinamico *dd = malloc(sizeof(TDicioDinamico));

  dd->hash = criarVetorDinamico();
  for (int i = 0; i <= tamanho; i++)
  {
    inserirVD(dd->hash, i, criarLSE());
  }
  dd->fc = fc;
  dd->tamanho = tamanho;

  dd->comparador = comparador;
  dd->impressor = impressor;
  return dd;
}

static int hashing(TDicioDinamico *dicio, int chave)
{
  /*
   * hash = chave / tamanho_do_dicionario
   */
  return chave % dicio->tamanho;
}

void inserirDD(TDicioDinamico *dicio, int chave, void *carga)
{
  int k = hashing(dicio, chave);

  /* Encontra a posicao do elemento */
  TListaSE *lst = acessarVD(dicio->hash, k);

  /* Sempre insere no comeco da lista encadeada */
  inserirInicioLSE(lst, carga);
}

void *buscarDD(TDicioDinamico *dicio, int chave)
{
  int k = hashing(dicio, chave);

  /* Encontra a posicao do elemento */
  TListaSE *lst = acessarVD(dicio->hash, k);

  /* Encontra o elemento desejado da lista encadeada */
  void *carga = buscarConteudoLSE(lst, dicio->comparador, &chave);

  return carga;
}

void *removerDD(TDicioDinamico *dicio, int chave)
{
  int k = hashing(dicio, chave);

  /* Encontra a posicao do elemento */
  TListaSE *lst = acessarVD(dicio->hash, k);

  /** Acha e remove o elemento da lista encadeada
   *  Retorna null se nao encontrar o elemento
   *  Retorna a nova lista escadeada se encontrar o elemnento
   */
  void *carga = removerConteudoLSE(lst, dicio->comparador, &chave);

  return carga;
}

void imprimirDD(TDicioDinamico *dicio)
{
  for (int i = 0; i < dicio->tamanho; i++)
  {
    TListaSE *lst = acessarVD(dicio->hash, i);
    printf("%d: ", i);
    imprimirLSE(lst, dicio->impressor);
  }
}
