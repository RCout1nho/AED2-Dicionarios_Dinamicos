/*
 * vd.c
 *
 *  Created on: 18 de out de 2017
 *      Author: cesar
 */

#include "stdlib.h"
#include "math.h"
#include "vdinamico.h"

struct vdinamico{
	void* *vetor;
	int ocupacao;
	int tamanho;
};

void trocar(void *a, void *b){
	//printf("%d %d\n", *a, *b);
	void *aux = a;
	a = b;
	b = aux;

	//printf("%d %d\n", *a, *b);
	//return;
}

int tamanhoVD(TVetorDinamico *vd){
	return vd->tamanho;
}

int ocupacaoVD(TVetorDinamico *vd){
	return vd->ocupacao;
}


void inserirVD(TVetorDinamico *vd, int idx, void *elem){

	//printf("A---%d %d\n", idx, vd->tamanho);
	if (idx >= vd->tamanho){
		vd->tamanho = (int)pow(2,floor(log2(idx))+1);
		//printf("D---%d %d\n", idx, vd->tamanho);
		vd->vetor = realloc(vd->vetor, sizeof(void*)*vd->tamanho);
	}

	vd->vetor[vd->ocupacao] = elem;
	vd->ocupacao++;
}

void *acessarVD(TVetorDinamico *vd, int k){
	void *elem = NULL;
	if ((k>=0) && (k < vd->tamanho)){
		elem = vd->vetor[k];
	}
	return elem;
}
void *removerVD(TVetorDinamico *vd, int idx){
	void *elem = NULL;
  if ((idx>=0) && (idx < vd->tamanho)){
		elem = vd->vetor[idx];
		vd->vetor[idx] = NULL;
	}
	if(elem != NULL){
		vd->ocupacao--;
	}

	return elem;
}

void destroyVD(TVetorDinamico *vd){
	for(int i=0;i<vd->tamanho;i++)
		if(vd->vetor[i]!=NULL)
			free(vd->vetor[i]);
	free(vd->vetor);
	vd->vetor=NULL;
	vd->tamanho=0;
	vd->ocupacao=0;
	free(vd);
}


//TVetorDinamico criarVetorDinamico(){

TVetorDinamico *criarVetorDinamico(){

	//TVetorDinamico vd=(TVetorDinamico){.vetor = NULL, .tamanho=2,.ocupacao=0};
	TVetorDinamico *vd = malloc(sizeof(TVetorDinamico));
	vd->ocupacao=0;
	vd->tamanho=2;
	vd->vetor = malloc(sizeof(void*) * vd->tamanho);

	return vd;
}
