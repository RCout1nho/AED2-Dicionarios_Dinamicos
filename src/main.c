#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "ctype.h"
#include "math.h"
#include "./ddinamico/ddinamico.h"
#include "./destatico/destatico.h"
#include "./lista/lista.h"
#include "./vdinamico/vdinamico.h"

typedef struct
{
  int chave;
  char *palavra;
  TListaSE *paginas;
} TPalavra;

typedef struct ocorrencia
{
  int pagina;
  int qtde;
} TOcorrencia;

typedef struct ocorrenciaImportancia
{
  double importancia;
  TOcorrencia *ocorrencia;
} TOcorrrenciaImportancia;

static int toInteiro(char palavra[])
{
  int pesos[] = {10, 455, 77, 91, 36, 44, 27, 45, 79, 68, 56};
  int chave = 0;
  for (int i = 0; (i < strlen(palavra) && i < 11); i++)
    chave += pesos[i] * palavra[i];

  return chave;
}

int compararOcorrencia(void *voc, void *chave)
{
  TOcorrencia *ocorre = voc;
  return ocorre->pagina - *((int *)chave);
}

static TOcorrencia *criarOcorrencia(int pagina)
{
  TOcorrencia *ocorre = malloc(sizeof(TOcorrencia));
  ocorre->pagina = pagina;
  ocorre->qtde = 1;
  return ocorre;
}

void imprimirOcorrencia(void *oc)
{
  TOcorrencia *ocorre = oc;
  printf("%d %d\n", ocorre->pagina, ocorre->qtde);
}

TPalavra *criarPalavra(char nova[], int pagina)
{
  TPalavra *p = malloc(sizeof(TPalavra));
  p->palavra = malloc(strlen(nova) + 1);

  strcpy(p->palavra, nova);
  p->chave = toInteiro(p->palavra);
  p->paginas = criarLSE();
  inserirInicioLSE(p->paginas, criarOcorrencia(pagina));
  return p;
}

void atualizarPalavra(TPalavra *palavra, int pagina)
{
  TOcorrencia *ocorre = buscarConteudoLSE(palavra->paginas, compararOcorrencia, &pagina);
  if (ocorre != NULL)
  {
    ocorre->qtde++;
  }
  else
  {
    inserirFinalLSE(palavra->paginas, criarOcorrencia(pagina));
  }
}

void mostrarPalavra(void *pa)
{
  TPalavra *p = pa;
  printf("%s\n", p->palavra);
  imprimirLSE(p->paginas, imprimirOcorrencia);
}

int compararPalavra(void *p, void *chave)
{
  TPalavra *pla = p;
  return (pla->chave - (*((int *)chave)));
}

static char *stristr(char *str, char *c)
{
  int i = 0;
  while ((*(str + i) != '\0') && (*(str + i) != *c))
  {
    i++;
  }
  if (*(str + i) != '\0')
  {
    return str + i;
  }
  else
  {
    return NULL;
  }
}
static void limpando(char *lida)
{
  int k = 0;
  char *limpa = malloc(strlen(lida) + 1);
  char pontuacao[] = ";???':!><.,?/`????????#%^&*-_+\\=)({}[]1234567890\"";
  int tam = strlen(lida);
  for (int i = 0; i < tam; i++)
  {
    if (stristr(pontuacao, &(lida[i])) == NULL)
    {
      limpa[k++] = tolower(lida[i]);
    }
    else if ((i > 0) && (i < tam - 1) && (lida[i] == '-'))
    {
      limpa[k++] = lida[i];
    }
  }
  limpa[k] = '\0';
  strcpy(lida, limpa);

  free(limpa);
}

char *lerpalavra()
{
  char lida[1000];
  char *lidaLimpa = NULL;
  int continua = 1;
  do
  {
    scanf("%s", lida);
    limpando(lida);
    int tam = strlen(lida);
    if (tam > 0)
    {
      lidaLimpa = malloc(sizeof(char) * (tam + 1));
      strcpy(lidaLimpa, lida);
      continua = 0;
    }
    else
    {
      if (feof(stdin))
      {
        continua = 0;
      }
    }
  } while (continua);

  return lidaLimpa;
}

char *criarStopword(char palavra[])
{
  char *newPalavra = malloc(sizeof(char *));

  strcpy(newPalavra, palavra);

  return newPalavra;
}

/**
 * @brief popula o dicion??rio est??tico de stop words
 *
 * @param dicio
 */
void preencher_stopword(TDicioEstatico *dicio)
{
  FILE *ptr;
  char str[50];
  ptr = fopen("src/stopwords_br_sorted.txt", "r");

  int i = 0;

  if (ptr == NULL)
  {
    return;
  }

  while (fscanf(ptr, "%s", str) != EOF)
  {
    inserirDE(dicio, i++, criarStopword(str));
  }

  fclose(ptr);
}

int compararStopword(void *elms, void *k)
{
  if (elms && k)
  {
    return strcmp((char *)elms, (char *)k);
  }

  return 0;
}

/**
 * @brief verifica se uma palavra ?? stopword
 *
 * @param dicio
 * @param k
 * @return int: 1 se for stopword | 0 se n??o for stopword
 */
int eh_stopword(TDicioEstatico *dicio, char *k)
{
  if (buscarBinariaDE(dicio, k, compararStopword) == NULL)
  {
    return 0;
  }

  return 1;
}

/**
 * @brief Computa "frequ??ncia de termo"
 *
 * @param word
 * @param pagina
 * @param dicio
 * @return double
 */
double tf(char *word, int pagina, TDicioDinamico *dicio)
{
  int chave = toInteiro(word);
  TPalavra *palavra = buscarDD(dicio, chave);

  TOcorrencia *ocorre = malloc(sizeof(TOcorrencia *));

  if (palavra)
  {
    ocorre = buscarConteudoLSE(palavra->paginas, compararOcorrencia, &pagina);
  }

  int totalDeOcorrenciasNoDoc = ocorre != NULL ? ocorre->qtde : 0; // numero de vezes que a palavra aparece na pagina

  int totalDePalavras = 0;

  for (int i = 0; i < 123; i++)
  {
    int j = 1;
    TPalavra *lst = retornaLista(dicio, i, j);

    while (lst != NULL)
    {
      j++;
      TOcorrencia *ocr = buscarConteudoLSE(lst->paginas, compararOcorrencia, &pagina);
      if (ocr != NULL)
      {
        totalDePalavras += ocr->qtde;
      }
      lst = retornaLista(dicio, i, j);
    }
  }

  return (totalDeOcorrenciasNoDoc * 1.0) / totalDePalavras;
}

/**
 * @brief Computa o n??mero de documentos que cont??m a palavra
 *
 * @param word
 * @param dicio
 * @return int
 */
int n_containing(char *word, TDicioDinamico *dicio)
{
  int chave = toInteiro(word);
  TPalavra *palavra = buscarDD(dicio, chave);

  if (palavra == NULL)
    return 0;

  int qtdPags = tamanhoLSE(palavra->paginas);

  return qtdPags;
}

/**
 * @brief computa "inverse document frequency"
 *
 * @param word
 * @param dicio
 * @param total_docs representa o n??mero total de paginas do documento
 * @return double
 */
double idf(char *word, TDicioDinamico *dicio, int total_docs)
{
  double r1 = (total_docs * 1.0) / (n_containing(word, dicio) + 1);
  return log10(r1);
}

/**
 * @brief Computa "TD-IDF"
 *
 * @param word
 * @param pagina
 * @param dicio
 * @param total_docs representa o n??mero total de paginas do documento
 * @return double
 */
double tfidf(char *word, int pagina, TDicioDinamico *dicio, int total_docs)
{
  return tf(word, pagina, dicio) * idf(word, dicio, total_docs);
}

/**
 * @brief Compara dois elementos do VD (Auxilia no processo de ordena????o)
 *
 * @param a
 * @param b
 * @return int
 */
int comparadorVD(void *a, void *b)
{
  TOcorrrenciaImportancia *ocr1 = a;
  TOcorrrenciaImportancia *ocr2 = b;

  if (ocr1->importancia == ocr2->importancia)
    return 0;
  if (ocr1->importancia < ocr2->importancia)
    return -1;
  if (ocr1->importancia > ocr2->importancia)
    return 1;
  return 0;
}

/**
 * @brief busca uma palavra
 *
 * @param word
 * @param dicio
 * @param total_docs total de p??ginas
 */
void buscarPalavra(char *word, TDicioDinamico *dicio, int total_docs)
{
  int chave = toInteiro(word);
  TPalavra *palavra = buscarDD(dicio, chave);

  /* Palavra n??o existe */
  if (palavra == NULL)
  {
    printf("Palavra n??o encontrada!\n");
    return;
  }

  /* Vetor din??mico que cont??m as ocorr??ncias da palavra */
  TVetorDinamico *vdOcorrencias = criarVetorDinamico();

  /* Quantidade de p??ginas que cont??m a palavra */
  int tam_paginas = tamanhoLSE(palavra->paginas);

  /**
   * Percorre todas as p??ginas
   * Calcula o tf-idf para cada p??gina
   * Salva a ocorr??ncia juntamente com sua import??ncia no VD
   */
  for (int i = 1; i <= tam_paginas; i++)
  {
    TOcorrrenciaImportancia *ocrImport = malloc(sizeof(TOcorrrenciaImportancia *));
    TOcorrencia *ocorrencia = acessarLSE(palavra->paginas, i);

    double importancia = tfidf(word, ocorrencia->pagina, dicio, total_docs);

    ocrImport->ocorrencia = ocorrencia;
    ocrImport->importancia = importancia;

    inserirVD(vdOcorrencias, i, ocrImport);
  }

  /* Ordena as ocorr??ncias pela import??ncia import??ncia */
  ordenaVD(vdOcorrencias, comparadorVD, -1);

  /* Quantidade de ocorr??ncias no VD */
  int tamanhoOcorrencias = ocupacaoVD(vdOcorrencias);

  /* No m??ximo 5 ocorr???ncias mais importantes, mas podem se menos */
  int qtdeMaisImportantes = tamanhoOcorrencias >= 5 ? 5 : tamanhoOcorrencias;

  printf("Palavra: %s\n", word);
  printf("Ocorr??ncias mais importantes:\n");
  for (int i = 0; i < qtdeMaisImportantes; i++)
  {
    TOcorrrenciaImportancia *ocr = acessarVD(vdOcorrencias, i);

    printf("Pagina: %d\n", ocr->ocorrencia->pagina);
    printf("Repeticoes: %d\n", ocr->ocorrencia->qtde);
    printf("Importancia: %.6f\n\n", ocr->importancia);
  }
}

int main(int argc, char const *argv[])
{
  /* Dicionario Dinamico de palavras em uma colecao de documentos */
  TDicioDinamico *docCollection = criarDicioDinamico(5, 123, compararPalavra, mostrarPalavra);

  /* Dicionario Estatico de stopwords */
  TDicioEstatico *stopwords = criarDicioEstatico();

  /* Popula o dicion??ro de stopwords */
  preencher_stopword(stopwords);

  int paginaAtual = 0;
  char *plida = lerpalavra();
  while (plida != NULL)
  {
    /* Pr??xima p??gina */
    if (strcmp(plida, "pa") == 0)
    {
      paginaAtual++;
    }
    else if (!eh_stopword(stopwords, plida)) // Verifica se a palavra n??o ?? stopword antes que index??-la
    {
      int chave = toInteiro(plida);
      if (chave > 0)
      {
        TPalavra *palavra = buscarDD(docCollection, chave);
        if (palavra == NULL)
        {
          inserirDD(docCollection, chave, criarPalavra(plida, paginaAtual));
        }
        else
        {
          atualizarPalavra(palavra, paginaAtual);
        }
      }
    }

    plida = lerpalavra();
  }
  // printf("tf-idf: %.6f", tfidf("bra??os", 1, docCollection, paginaAtual));
  buscarPalavra("bra??os", docCollection, paginaAtual);
  // imprimirDD(docCollection);
  return 0;
}
