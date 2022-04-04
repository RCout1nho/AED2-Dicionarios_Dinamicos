#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "ctype.h"
#include "./ddinamico/ddinamico.h"
#include "./destatico/destatico.h"
#include "./obasico/lista.h"
#include "./obasico/vdinamico.h"

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
  char pontuacao[] = ";’':!><.,?/`”“˜#%^&*-_+\\=)({}[]1234567890\"";
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
 * # Esta função é responsável por popular o dicionário estático
 * de stopwords com o conteúdo do arquivo de stopwords
 *
 * O conteúdo do arquivo foi preveamente ordenado (comom sugere o documento do trabalho)
 *
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

int eh_stopword(TDicioEstatico *dicio, char *k)
{
  if (buscarBinariaDE(dicio, k, compararStopword) == NULL)
  {
    return 0;
  }

  return 1;
}

/**
 * # Esta função é uma tentativa de implementar a função que
 * computa a "frequência de tempo"
 *
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

  free(ocorre);

  printf("Total de palavras na página %d: %d %d\n", pagina, totalDeOcorrenciasNoDoc, totalDePalavras);

  return (totalDeOcorrenciasNoDoc * 1.0) / totalDePalavras;
}

int main(int argc, char const *argv[])
{
  /* Dicionario Dinamico de palavras em uma colecao de documentos */
  TDicioDinamico *docCollection = criarDicioDinamico(5, 123, compararPalavra, mostrarPalavra);

  /* Dicionario Estatico de stopwords */
  TDicioEstatico *stopwords = criarDicioEstatico();

  /* Popula o dicionáro de stopwords */
  preencher_stopword(stopwords);

  int paginaAtual = 0;
  char *plida = lerpalavra();
  while (plida != NULL)
  {
    /* Próxima página */
    if (strcmp(plida, "pa") == 0)
    {
      paginaAtual++;
    }
    else if (!eh_stopword(stopwords, plida)) // Verifica se a palavra não é stopword antes que indexá-la
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
  imprimirDD(docCollection);
  // printf("tf: %.8f\n", tf("quantos", 230, docCollection));
  return 0;
}
