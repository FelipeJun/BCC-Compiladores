#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>

typedef enum
{
  Mais,
  Menos,
  Multiplicacao,
  Divisao,
  Potenciacao,
  Numero,
  Indeterminado
} Token;

typedef struct
{
  Token tipo;
  char *valor;
} TokenInfo;

TokenInfo *criarNovoToken(Token tipo, char **valor)
{
  TokenInfo *token = (TokenInfo *)malloc(sizeof(TokenInfo));
  token->tipo = tipo;
  token->valor = (char *)malloc(strlen(*valor));
  strcpy(token->valor, *valor);
  return token;
}

bool checarPotencia(char ***charAtual)
{
  char cAtual = ***charAtual;
  char cProx = *(**charAtual + 1);
  if (cAtual == cProx && cProx != '\0')
    return true;
  else
    return false;
}

bool checarNumerico(char charAtual)
{
  if ((isdigit(charAtual) || charAtual == '.') && charAtual != '\0' && charAtual != ' ' && charAtual != '\t')
    return true;
  else
    return false;
}

char *checarNumeros(char ***charAtual)
{
  char *copia = strdup(**charAtual);
  char *vetor = (char *)malloc(sizeof(char) * 24);
  char cAtual = *copia;
  int i = 0;
  while (checarNumerico(cAtual))
  {
    vetor[i] = cAtual;
    i++;
    cAtual = *(++(copia));
  }
  vetor[i] = '\0';
  free(copia);
  return vetor;
}

TokenInfo *ProxToken(char **charAtual)
{
  char *c = (char *)malloc(sizeof(char));
  Token tipo = Indeterminado;
  int jumper = 0;
  if (isdigit(**charAtual))
  {
    tipo = Numero;
    char *valor = checarNumeros(&charAtual);
    jumper = strlen(valor);
    *c = (char *)realloc(c, sizeof(char) * (jumper));
    strcpy(c, valor);
    (*charAtual) += jumper;
    free(valor);
  }
  else
  {
    switch (**charAtual)
    {
    case '+':
      tipo = Mais;
      break;
    case '-':
      tipo = Menos;
      break;
    case '*':
      if (checarPotencia(&charAtual))
      {
        tipo = Potenciacao;
        (*charAtual)++;
      }
      else
        tipo = Multiplicacao;
      *c = **charAtual;
      break;
    case '/':
      tipo = Divisao;
      break;
    default:
      (*charAtual)++;
      return NULL;
      break;
    }

    (*charAtual)++;
  }

  TokenInfo *token = criarNovoToken(tipo, &c);
  free(c);
  return token;
}

void tokenizer(char *s, TokenInfo **tokens, int *tamanho)
{
  *tokens = (TokenInfo *)malloc(sizeof(TokenInfo) * (strlen(s) + 1));
  *tamanho = 0;

  char *charAtual = s;
  while (*charAtual)
  {
    TokenInfo *token = ProxToken(&charAtual);
    if (token != NULL)
    {
      (*tokens)[*tamanho] = *token;
      (*tamanho)++;
      free(token);
    }
  }
}

bool parserChecker(Token tipo){
  if(tipo == Numero)
    return true;
  perror("Termo Invalido em arquivo\n");
  exit(1);
}


double parser(TokenInfo **tokens, int *tamanho)
{
  double expressao = 0;
  double tolerancia = 0.0000001;
  if (*tamanho > 0)
  {
    if ((*tokens)[0].tipo != Numero)
    {
      perror("Primeiro token nao e um numero\n");
      exit(1);
    }
    for (int i = 0; i < *tamanho; i++)
    {
      switch ((*tokens)[i].tipo)
      {
      case Mais:
        if(parserChecker((*tokens)[i + 1].tipo))
          expressao +=atof((*tokens)[i + 1].valor);
        break;
      case Menos:
        if(parserChecker((*tokens)[i + 1].tipo))
          expressao -=atof((*tokens)[i + 1].valor);
        break;
      case Multiplicacao:
        if(parserChecker((*tokens)[i + 1].tipo))
          expressao *=atof((*tokens)[i + 1].valor);
        break;
      case Divisao:
        if(parserChecker((*tokens)[i + 1].tipo))
          expressao /=atof((*tokens)[i + 1].valor);
        break;
      case Potenciacao:
        if(parserChecker((*tokens)[i + 1].tipo))
          expressao = pow(expressao,atof((*tokens)[i + 1].valor));
        break;
      case Numero:
        if (fabs(expressao) <= tolerancia){
          expressao =atof((*tokens)[i].valor);
        }
        break;
      case Indeterminado:
        perror("Arquivo invalido\n");
        exit(1);
        break;
      }
    }
  }
  return expressao;
}

int main(int argc, char *argv[])
{
  FILE *file;
  long size;
  char *textoArquivo;
  double expressao = 0;
  file = fopen(argv[1], "rb");
  if (!file)
  {
    perror("Falha ao abrir o arquivo");
    exit(1);
  }

  fseek(file, 0, SEEK_END);
  size = ftell(file);
  fseek(file, 0, SEEK_SET);

  textoArquivo = (char *)malloc(size + 1);
  if (!textoArquivo)
  {
    perror("Falha ao alocar memória");
    fclose(file);
    exit(1);
  }

  fread(textoArquivo, 1, size, file);
  textoArquivo[size] = '\0';

  fclose(file);

  printf("ARQUIVO: %s\n", textoArquivo);
  for (int i = 0; i < 50; i++)
    printf("-");
  printf("\n");
  
  TokenInfo *tokens;
  int tamanho;
  tokenizer(textoArquivo, &tokens, &tamanho);
  printf("LEITURA DE TOKENS:\n");
  for (int i = 0; i < tamanho; i++)
  {
    switch (tokens[i].tipo)
    {
    case Mais:
      printf("Mais\n");
      break;
    case Menos:
      printf("Menos\n");
      break;
    case Multiplicacao:
      printf("Multiplicacao\n");
      break;
    case Divisao:
      printf("Divisao\n");
      break;
    case Potenciacao:
      printf("Potenciacao\n");
      break;
    case Numero:
      printf("Numero: %s\n", tokens[i].valor);
      break;
    case Indeterminado:
      printf("Indertemidado: %s\n", tokens[i].valor);
      break;
    }
  }
  expressao = parser(&tokens, &tamanho);
  if (expressao == 0)
    printf("Resultado: Vazio\n");
  else
    printf("Resultado: %.2f\n",expressao);
  free(tokens);
  free(textoArquivo);
  return 0;
}