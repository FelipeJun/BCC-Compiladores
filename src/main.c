#include "funcoes.h"
// padrao do binario {42,0,0{load}}
// 42 obrigatorio sendo o primeiro, posição 1 é o N e a seguinte é o Z

//Exemplo para Soma: uint8_t buffer[12] = {42,0,0,32,10,48,11,16,1,240,3,5};
//Exemplo para subtração: uint8_t buffer[14] = {42,0,0,32,12,96,48,13,96,16,1,240,10,4};
//Exemplo para Jmp: uint8_t buffer[10] = {42,0,0,128,7,1,2,3,240,0,1};

void criarBinario(){
	uint8_t buffer[11] = {42,0,0,128,7,1,2,3,240,0,1};
	FILE *file = fopen("binario.bin", "wb");
	if (file == NULL)
	{
		printf("Erro ao abrir o arquivo\n");
	}

	fwrite(buffer,1, 11, file);
	fclose(file);
	printf("Binario Criado!\n");
}

int main() {
	criarBinario();
	FILE *file;
	file = fopen("binario.bin", "r");
	if (file == NULL)
	{
		printf("Erro ao abrir o argv[1]!\n");
		return 1;
	}
	
	binary(file);
	fclose(file);
	return 0;
}