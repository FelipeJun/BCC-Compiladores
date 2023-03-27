#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { 
	Mais, 
	Menos, 
	Indeterminado 
} Token;

typedef struct {
  Token tipo;
  char valor;
} TokenInfo;

TokenInfo *criarNovoToken(Token tipo, char valor) {
  TokenInfo *token = (TokenInfo *)malloc(sizeof(TokenInfo));
  token->tipo = tipo;
  token->valor = valor;
  return token;
}

TokenInfo *ProxToken(char **charAtual) {
  char c = **charAtual;
  Token tipo = Indeterminado;
  switch (c) {
  case '+':
    tipo = Mais;
    (*charAtual)++;
    break;
  case '-':
    tipo = Menos;
    (*charAtual)++;
    break;
  case ' ':
    (*charAtual)++;
    return NULL;
  default:
    (*charAtual)++;
    break;
  }
  TokenInfo *token = criarNovoToken(tipo, c);
  return token;
}

void tokenizer(char *s, TokenInfo **tokens, int *tamanho) {
  *tokens = (TokenInfo *)malloc(sizeof(TokenInfo) * strlen(s));
  *tamanho = 0;

  char *charAtual = s;
  while (*charAtual) {
    TokenInfo *token = ProxToken(&charAtual);
    if (token != NULL) {
      (*tokens)[*tamanho] = *token;
      (*tamanho)++;
      free(token);
    }
  }
}

int main(void) {
  char s[]= "+-+ *???*+--+q12-";
	TokenInfo* tokens;
	int tamanho;
	tokenizer(s, &tokens, &tamanho);
	for (int i = 0; i < tamanho; i++) {
			switch (tokens[i].tipo) {
					case Mais:
							printf("Mais\n");
							break;
					case Menos:
							printf("Menos\n");
							break;
					case Indeterminado:
							printf("Indertemidado: %c\n", tokens[i].valor);
							break;
			}
	}
	free(tokens);
	return 0;
}