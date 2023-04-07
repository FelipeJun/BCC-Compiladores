#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

typedef enum
{
  Mais,
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
    *c = (char *)realloc(c, sizeof(char) * (jumper + 1));
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
    case ' ':
      (*charAtual)++;
      return NULL;
    case '\t':
      (*charAtual)++;
      return NULL;
    default:
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

void parser(TokenInfo **tokens, int *tamanho)
{
  int expressao = 0;
  int size = 0;
  if (*tamanho > 0)
  {
    if ((*tokens)[0].tipo != Numero)
    {
      perror("PRIMEIRO TOKEN NAO E NUMERO\n");
      return 1;
    }
    for (int i = 0; i < *tamanho; i++)
    {
      switch ((*tokens)[i].tipo)
      {
      case Mais:
        if((*tokens)[i + 1].tipo == Numero){
          expressao +=atoi((*tokens)[i + 1].valor);
        }
        break;
      case Numero:
        if (expressao == 0){
          expressao =atoi((*tokens)[i].valor);
        }
        break;
      case Indeterminado:
        perror("ARQUIVO INVALIDO\n");
        return 1;
        break;
      }
    }
  }
  else
  {
    printf("Resultado: Vazio\n");
  }
  printf("RESULTADO %d",expressao);
}

int main(int argc, char *argv[])
{
  FILE *file;
  long size;
  char *textoArquivo;
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

  printf("Arquivo: %s\n", textoArquivo);

  TokenInfo *tokens;
  int tamanho;
  tokenizer(textoArquivo, &tokens, &tamanho);
  for (int i = 0; i < tamanho; i++)
  {
    switch (tokens[i].tipo)
    {
    case Mais:
      printf("Mais\n");
      break;
    case Numero:
      printf("Numero: %s\n", tokens[i].valor);
      break;
    case Indeterminado:
      printf("Indertemidado: %s\n", tokens[i].valor);
      break;
    }
  }
  parser(&tokens, &tamanho);
  free(tokens);
  free(textoArquivo);
  return 0;
}