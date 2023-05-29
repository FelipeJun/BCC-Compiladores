#include "funcoes.h"
#include <string.h>
// padrao do binario {42,0,0{load}}
// 42 obrigatorio sendo o primeiro, posição 1 é o N e a seguinte é o Z

//Exemplo para Soma: 	  uint8_t buffer[12] =  {42,0,0,32,10,48,11,16,1,240,3,5};
//Exemplo para subtração: uint8_t buffer[14] =  {42,0,0,32,12,96,48,13,96,16,1,240,10,4};
//Exemplo para Jmp:		  uint8_t buffer[13] =  {42,0,0,128,5,0,128,9,0,128,2,0,240,0};
//Exemplo para OR:		  uint8_t buffer[13] =  {42,0,0,32,10,64,11,16,1,240,1,2,0};
//Exemplo para AND:       uint8_t buffer[13] =  {42,0,0,32,10,80,11,16,1,240,3,2,0};
//Exemplo para JN pulando:uint8_t buffer[13] =  {42,0,0,32,10,144,10,0,0,240,250,0,240};
//Exemplo para JN:        uint8_t buffer[13] =  {42,0,0,32,10,144,8,0,0,240,5,0,240};
//Exemplo para JZ pulando:uint8_t buffer[13] =  {42,0,0,32,11,160,8,240,0,0,240,5,0};
//Exemplo para JZ:        uint8_t buffer[13] =  {42,0,0,32,11,160,8,240,0,0,240,0,0};


void criarBinario(){
	uint8_t buffer[14] =  {42,0,0,32,12,96,48,13,96,16,1,240,10,4};
	FILE *file = fopen("teste.nar", "wb");
	if (file == NULL)
	{
		printf("Erro ao abrir o arquivo\n");
	}

	fwrite(buffer,1, 14, file);
	fclose(file);
	printf("Binario Criado!\n");
}

int main(int argc, char *argv[]) {
	if (argc != 2) {
        printf("Uso: %s nome_do_arquivo\n", argv[0]);
        return 1;
    }
	// criarBinario();
	FILE *file;
	file = fopen(argv[1], "r");
	if (file == NULL)
	{
		printf("Erro ao abrir o argv[1]!\n");
		return 1;
	}
	char *extensao = strrchr(argv[1], '.');
	if (extensao == NULL) {
        printf("Arquivo sem extensao.\n");
        return 1;
    }
	if (strcmp(extensao, ".nar") != 0) {
		printf("Arquivo com extensao desconhecida.\n");
		return 1;
	}
	
	binary(file);
	fclose(file);
	return 0;
}