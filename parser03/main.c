#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>

#define TAMANHO_MAX_TEXTO 255

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

bool checarPotencia(char *charAtual)
{
  char cAtual = *charAtual;
  char cProx = *(++(charAtual));
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

char *checarNumeros(char *charAtual)
{
  char *copia = charAtual;
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
  return vetor;
}

TokenInfo *ProxToken(char **charAtual)
{
  char *c = (char *)malloc(sizeof(char));
  Token tipo = Indeterminado;
  int jumper = 0;
  char *cAtual = *charAtual;
  if (isdigit(**charAtual))
  {
    tipo = Numero;
    char *valor = checarNumeros(cAtual);
    jumper = strlen(valor);
    c = (char *)realloc(c, sizeof(char) * (jumper + 1));
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
      if (checarPotencia(cAtual))
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
    case ' ':
      (*charAtual)++;
      return NULL;
    case '\t':
      (*charAtual)++;
      return NULL;
    case '\n':
      (*charAtual)++;
      return NULL;
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

bool parserChecker(Token tipo)
{
  if (tipo == Numero)
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
        if (parserChecker((*tokens)[i + 1].tipo))
          expressao += atof((*tokens)[i + 1].valor);
        break;
      case Menos:
        if (parserChecker((*tokens)[i + 1].tipo))
          expressao -= atof((*tokens)[i + 1].valor);
        break;
      case Multiplicacao:
        if (parserChecker((*tokens)[i + 1].tipo))
          expressao *= atof((*tokens)[i + 1].valor);
        break;
      case Divisao:
        if (parserChecker((*tokens)[i + 1].tipo))
          expressao /= atof((*tokens)[i + 1].valor);
        break;
      case Potenciacao:
        if (parserChecker((*tokens)[i + 1].tipo))
          expressao = pow(expressao, atof((*tokens)[i + 1].valor));
        break;
      case Numero:
        if (fabs(expressao) <= tolerancia)
        {
          expressao = atof((*tokens)[i].valor);
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

bool checkInputCommands(char *texto)
{
  if (texto[0] == ':')
  {
    if (strcmp(texto+1, "quit\n") == 0 || strcmp(texto+1, "exit\n") == 0)
    {
      return true;
    }
    else
    {
      printf("Comando invalido!\n");
    }
  }
  return false;
}

int main()
{
  bool exit = false;
  char textoArquivo[TAMANHO_MAX_TEXTO];
  double expressao = 0;
  TokenInfo *tokens;
  int tamanho;
  printf("Bem Vindo!\nDigite a expressao desejada, ou digite :quit para fechar o programa\n");
  while (1)
  {
    fgets(textoArquivo, TAMANHO_MAX_TEXTO, stdin);
    exit = checkInputCommands(textoArquivo);
    if (exit)
      break;
    tokenizer(textoArquivo, &tokens, &tamanho);
    expressao = parser(&tokens, &tamanho);
    if (expressao == 0)
      printf("Resultado: Vazio\n");
    else
      printf("Resultado: %.2f\n", expressao);
    free(tokens);
  }

  return 0;
}