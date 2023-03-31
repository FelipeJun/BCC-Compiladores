#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define TAMANHO 255

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
  char valor[TAMANHO];
} TokenInfo;

TokenInfo *criarNovoToken(Token tipo, char*valor, bool numberChecker)
{
  TokenInfo *token = (TokenInfo *)malloc(sizeof(TokenInfo));
  token->tipo = tipo;
  token->valor = valor;
  

  return token;
}

bool checarPotencia(char ***charAtual)
{
  char cAtual = ***charAtual;
  // printf("cAtual: %c\n",cAtual);
  char cProx = *(**charAtual + 1);
  // printf("cProx: %c\n",cProx);
  if (cAtual == cProx && cProx != '\0')
    return true;
  else
    return false;
}

/*
Nesta nova função, tem que fazer um while q vai fica rodando até o numero acabar, 
e retorar o valor de quanto ele vai pular na função proxToken

Tambem tem que ver como armazenar o valor do numero
*/
int checarNumeros(char ***charAtual){

  char cAtual = ***charAtual;
  char cProx = *(**charAtual + 1);
  if(isdigit(cProx) || cProx == '.'){

  }

}

TokenInfo *ProxToken(char **charAtual)
{
  char c = **charAtual;
  Token tipo = Indeterminado;
  if (isdigit(c))
  {
    int jumpChars = checarNumeros(&charAtual);
    tipo = Numero;
    (*charAtual)+= jumpChars;
  }
  else
  {
    switch (c)
    {
    case '+':
      tipo = Mais;
      (*charAtual)++;
      break;
    case '-':
      tipo = Menos;
      (*charAtual)++;
      break;
    case '*':
      if (checarPotencia(&charAtual))
      {
        tipo = Potenciacao;
        (*charAtual)++;
      }
      else
      {
        if ((*(*charAtual + 1)) != '\0')
          tipo = Multiplicacao;
        else
        {
          (*charAtual)++;
          return NULL;
        }
      }
      (*charAtual)++;
      break;
    case '/':
      tipo = Divisao;
      (*charAtual)++;
      break;
    case ' ':
      (*charAtual)++;
      return NULL;
    case '\t':
      (*charAtual)++;
      return NULL;
    default:
      (*charAtual)++;
      break;
    }
  }
  TokenInfo *token = criarNovoToken(tipo, c);
  return token;
}

void tokenizer(char *s, TokenInfo **tokens, int *tamanho)
{
  *tokens = (TokenInfo *)malloc(sizeof(TokenInfo) * strlen(s));
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

  printf("%s\n", textoArquivo);

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
      printf("Numero: %c\n", tokens[i].valor);
      break;
    case Indeterminado:
      printf("Indertemidado: %c\n", tokens[i].valor);
      break;
    }
  }
  free(tokens);
  free(textoArquivo);
  return 0;
}