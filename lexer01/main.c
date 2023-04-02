#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

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

TokenInfo *criarNovoToken(Token tipo, char *valor)
{
  TokenInfo *token = (TokenInfo *)malloc(sizeof(TokenInfo));
  token->tipo = tipo;
  token->valor = (char *)malloc(strlen(valor) + 1);
  strcpy(token->valor, valor);
  

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

/*
Nesta nova função, tem que fazer um while q vai fica rodando até o numero acabar, 
e retorar o valor de quanto ele vai pular na função proxToken

Tambem tem que ver como armazenar o valor do numero
*/
void checarNumeros(char ***charAtual){
  printf("ENTROU NA FUNÇÃO\n");
  // char* vetor = malloc(TAMANHO * sizeof(char));

  char cAtual = ***charAtual;
  char cProx;
  printf("Atual %c",cAtual);
  while(isdigit(cAtual) || cAtual == '.'){
    // strcpy(vetor, cAtual);
    cProx = *(**charAtual + 1);
    printf("Atual %c e cProx %c",cAtual,cProx);
  }
  // printf("%s",vetor);
}

TokenInfo *ProxToken(char **charAtual)
{
  char c = **charAtual;
  Token tipo = Indeterminado;
  if (isdigit(c))
  {
    // char* valor = checarNumeros(&charAtual);
    tipo = Numero;
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
      break;
    case '/':
      tipo = Divisao;
      break;
    case ' ':
      return NULL;
    case '\t':
      return NULL;
    default:
      break;
    }
    (*charAtual)++;
  }
  TokenInfo *token = criarNovoToken(tipo, &c);
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