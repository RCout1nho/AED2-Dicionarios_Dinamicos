/*
 * vdinamico.h
 *
 *  Created on: 18 de out de 2017
 *      Author: cesar
 *
 *  Alterações adicionadas:
 *   + ordernaVD
 */
typedef struct vdinamico TVetorDinamico;
typedef int (*TComparadorDD)(void *a, void *b);

int tamanhoVD(TVetorDinamico *vd);
int ocupacaoVD(TVetorDinamico *vd);

void *acessarVD(TVetorDinamico *vd, int idx);
void *removerVD(TVetorDinamico *vd, int idx);

void inserirVD(TVetorDinamico *vd, int idx, void *elem);
void destroyVD(TVetorDinamico *vd);

void ordenaVD(TVetorDinamico *vd, TComparadorDD comparador, int ordem);

TVetorDinamico *criarVetorDinamico();
