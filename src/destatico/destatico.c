#include "stdio.h"
#include "stdlib.h"
#include "../obasico/vdinamico.h"
#include "destatico.h"

/* TAD Dicionario */
struct dicio
{
  /* vetor dinamico */
  TVetorDinamico *elms;
};

TDicioEstatico *criarDicioEstatico()
{
  TDicioEstatico *de = malloc(sizeof(TDicioEstatico));
  de->elms = criarVetorDinamico();

  return de;
}

void inserirDE(TDicioEstatico *dicio, int idx, void *elm)
{
  TVetorDinamico *elms = dicio->elms;
  inserirVD(elms, idx, elm);
}

void *buscarSequencialDE(TDicioEstatico *dicio, void *chave, TCompararDE comparar)
{
  TVetorDinamico *elms = dicio->elms;
  int tam = tamanhoVD(elms);
  int i = 0;
  void *elm = NULL;
  int achou = 0;
  while ((!achou) && (i < tam))
  {
    elm = acessarVD(elms, i);
    if (comparar(elm, chave) == 0)
    {
      achou = 1;
    }
    else
    {
      i++;
    }
  }
  if (achou)
  {
    return elm;
  }
  else
  {
    return NULL;
  }
}

void *buscarBinariaDE(TDicioEstatico *dicio, void *chave, TCompararDE comparar)
{
  TVetorDinamico *elms = dicio->elms;
  int tam = tamanhoVD(elms);
  int i = 0, j = tam - 1;
  int achou = 0;
  void *elm = NULL;
  while ((!achou) && (j - i) >= 0)
  {
    int meio = (j + i) / 2;
    elm = acessarVD(elms, meio);
    int retorno = comparar(elm, chave);
    if (retorno == 0)
    {
      achou = 1;
    }
    else if (retorno > 0)
    {
      j = meio - 1;
    }
    else
    {
      i = meio + 1;
    }
  }
  if (!achou)
  {
    return NULL;
  }
  else
  {
    return elm;
  }
}
