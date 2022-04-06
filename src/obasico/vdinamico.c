/*
 * vd.c
 *
 *  Created on: 18 de out de 2017
 *      Author: cesar
 *  Alterações adicionadas:
 *   + ordernaVD
 */

#include "stdlib.h"
#include "math.h"
#include "vdinamico.h"
#include "assert.h"
#include "string.h"

struct vdinamico
{
	void **vetor;
	int ocupacao;
	int tamanho;
};

static void troca(void *a, void *b, size_t size)
{
	void *temp = malloc(size);

	assert(temp != NULL);

	memcpy(temp, b, size);
	memcpy(b, a, size);
	memcpy(a, temp, size);

	free(temp);
}

static int particao(TVetorDinamico *vd, int esquerda, int direita, TComparadorDD comparador, int ordem)
{
	void *pivot = vd->vetor[direita];

	int i = esquerda - 1;

	for (int j = esquerda; j < direita; j++)
	{
		if (comparador(vd->vetor[j], pivot) == -ordem)
		{
			i++;

			troca(&vd->vetor[i], &vd->vetor[j], sizeof(vd->vetor[i]));
		}
	}

	troca(&vd->vetor[i + 1], &vd->vetor[direita], sizeof(vd->vetor[i + 1]));

	return (i + 1);
}

static void quickSort(TVetorDinamico *vd, int esquerda, int direita, TComparadorDD comparador, int ordem)
{
	if (esquerda < direita)
	{
		int pi = particao(vd, esquerda, direita, comparador, ordem);

		quickSort(vd, esquerda, pi - 1, comparador, ordem);

		quickSort(vd, pi + 1, direita, comparador, ordem);
	}
}

/**
 * @brief ordena vetor dinâmico
 *
 * @param vd
 * @param comparador
 * @param ordem crescente = 1 | decrescente = -1
 */
void ordenaVD(TVetorDinamico *vd, TComparadorDD comparador, int ordem)
{
	quickSort(vd, 0, vd->ocupacao - 1, comparador, ordem);
}

int tamanhoVD(TVetorDinamico *vd)
{
	return vd->tamanho;
}

int ocupacaoVD(TVetorDinamico *vd)
{
	return vd->ocupacao;
}

void inserirVD(TVetorDinamico *vd, int idx, void *elem)
{

	if (idx >= vd->tamanho)
	{
		vd->tamanho = (int)pow(2, floor(log2(idx)) + 1);
		vd->vetor = realloc(vd->vetor, sizeof(void *) * vd->tamanho);
	}

	vd->vetor[vd->ocupacao] = elem;
	vd->ocupacao++;
}

void *acessarVD(TVetorDinamico *vd, int k)
{
	void *elem = NULL;
	if ((k >= 0) && (k < vd->tamanho))
	{
		elem = vd->vetor[k];
	}
	return elem;
}
void *removerVD(TVetorDinamico *vd, int idx)
{
	void *elem = NULL;
	if ((idx >= 0) && (idx < vd->tamanho))
	{
		elem = vd->vetor[idx];
		vd->vetor[idx] = NULL;
	}
	if (elem != NULL)
	{
		vd->ocupacao--;
	}

	return elem;
}

void destroyVD(TVetorDinamico *vd)
{
	for (int i = 0; i < vd->tamanho; i++)
		if (vd->vetor[i] != NULL)
			free(vd->vetor[i]);
	free(vd->vetor);
	vd->vetor = NULL;
	vd->tamanho = 0;
	vd->ocupacao = 0;
	free(vd);
}

TVetorDinamico *criarVetorDinamico()
{

	TVetorDinamico *vd = malloc(sizeof(TVetorDinamico));
	vd->ocupacao = 0;
	vd->tamanho = 2;
	vd->vetor = malloc(sizeof(void *) * vd->tamanho);

	return vd;
}
