#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "ctype.h"
#include "ddinamico.h"
#include "./obasico/lista.h"

#define STOPWORDS_TOTAL 392
#define STOPWORD_SIZE 13

typedef struct
{
  int chave;
  char *palavra;
  TListaSE *paginas;
  // char definicao[2000];
} TPalavra;

static int toInteiro(char palavra[])
{
  int pesos[] = {10, 455, 77, 91, 36, 44, 27, 45, 79, 68, 56};
  int chave = 0;
  //  assert(strlen(palavra)<11);
  //  for (int i=0;i<strlen(palavra);i++)
  for (int i = 0; (i < strlen(palavra) && i < 11); i++)
    chave += pesos[i] * palavra[i];

  return chave;
}

typedef struct ocorrencia
{
  int pagina;
  int qtde;
} TOcorrencia;

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

void preenche_stopword(char stopwords[STOPWORDS_TOTAL][STOPWORD_SIZE])
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
    strcpy(stopwords[i++], str);
  }

  fclose(ptr);
}

int busca_binaria(char word[STOPWORD_SIZE], char stopwords[STOPWORDS_TOTAL][STOPWORD_SIZE], int inicio, int fim)
{
  int meio, comp;

  if (inicio > fim)
    return 0;

  meio = (inicio + fim) / 2;

  comp = strcmp(word, stopwords[meio]);

  if (comp == 0)
  {
    return 1;
  }
  if (comp < 0)
  {
    return busca_binaria(word, stopwords, inicio, meio - 1);
  }
  else
  {
    return busca_binaria(word, stopwords, meio + 1, fim);
  }
}

int eh_stopword(char word[STOPWORD_SIZE], char stopwords[STOPWORDS_TOTAL][STOPWORD_SIZE])
{
  return busca_binaria(word, stopwords, 0, STOPWORDS_TOTAL - 1);
}

int main(int argc, char const *argv[])
{
  TDicioDinamico *dicio = criarDicioDinamico(2, 17, compararPalavra, mostrarPalavra);

  char stopwords[STOPWORDS_TOTAL][STOPWORD_SIZE];
  preenche_stopword(stopwords);

  int paginaAtual = 0;
  char *plida = lerpalavra();
  while (plida != NULL)
  {
    /* Próxima página */
    if (strcmp(plida, "pa") == 0)
    {
      paginaAtual++;
    }
    else if (!eh_stopword(plida, stopwords))
    {
      /* Se não for stopword */
      int chave = toInteiro(plida);
      if (chave > 0)
      {
        // TPalavra *palavra=NULL;
        TPalavra *palavra = buscarDD(dicio, chave);
        if (palavra == NULL) // Não há colisão
        {
          inserirDD(dicio, chave, criarPalavra(plida, paginaAtual));
        }
        else // Há colisão
        {
          atualizarPalavra(palavra, paginaAtual);
        }
      }
    }
    plida = lerpalavra();
  }
  imprimirDD(dicio);
  return 0;
}
