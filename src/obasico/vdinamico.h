/*
 * vdinamico.h
 *
 *  Created on: 18 de out de 2017
 *      Author: cesar
 */

typedef struct vdinamico TVetorDinamico;

int tamanhoVD(TVetorDinamico *vd);
int ocupacaoVD(TVetorDinamico *vd);

void *acessarVD(TVetorDinamico *vd, int idx);
void *removerVD(TVetorDinamico *vd, int idx);

void inserirVD(TVetorDinamico *vd, int idx, void *elem);
void destroyVD(TVetorDinamico *vd);

TVetorDinamico *criarVetorDinamico();
