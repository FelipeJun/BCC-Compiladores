#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

#define MAX_SIZE 255

//Mnemônicos
//parar programa
#define HLT 240
//Salvar em byte
#define STA 16
//Ler byte
#define LDA 32
//Adicionar
#define ADD 48
#define OR 64
#define AND 80
#define NOT 96
#define JMP 128
#define JN 144
#define JZ 160


void checkByte(uint8_t buffer [],uint8_t *Ac, uint8_t *Pc,int *tamanho){
	//{ 0,1,2, 3, 4, 5, 6, 7,8, 8,10,11}
	//{42,0,0,32,10,48,11,16,1,240,3,5}
	uint8_t indexAtual = *tamanho;
	uint8_t proxIndex = (++(indexAtual));
	switch(buffer[*tamanho]){
		case HLT:
			printf("Resultado final:\nAC: %d\nPC: %d",*Ac,*Pc);
			exit(0);
		break;
		case STA:
			printf("STA");
		break;
		case LDA:
			printf("LDA");
			*Ac = buffer[buffer[proxIndex]];
			(*tamanho)++;
		break;
		case ADD:
			printf("ADD");
			*Ac += buffer[buffer[proxIndex]];
			(*tamanho)++;
		break;
		case OR:
			printf("OR");
		break;
		case AND:
			printf("AND");
		break;
		case NOT:
			printf("NOT");
			*Ac = ~*Ac;
		break;
		case JMP:
			printf("JMP");
		break;
		case JN:
			printf("JN");
		break;
		case JZ:
			printf("JZ");
		break;
	}
	(++(*tamanho));
	printf("\n");
}

int binary(FILE *file){
	
	uint8_t buffer [MAX_SIZE];
	uint8_t AC, PC = 0;
	int i = 0;
	fseek(file, 0, SEEK_SET);
	fread(&buffer, MAX_SIZE, 1, file);
	if(buffer[0] != 42){
		printf("Binario não aceito\n");
		return 1;
	}
	else{
		AC = buffer[1];
		PC = buffer[2];
		while(1){
			PC = i;
			checkByte(buffer,&AC,&PC,&i);
		}
	}
	return 0;
}
