#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <stdbool.h>

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

bool verificaNegativo(uint8_t num){
  return num >> 7;
}

void checkByte(uint8_t buffer [],uint8_t *Ac,uint8_t *Pc,uint8_t *tamanho){
	uint8_t proxIndex= *tamanho;
	uint8_t indexAtual = (proxIndex++);
	// printf("\nbuffer[%d] = %d",indexAtual,buffer[indexAtual]);
	// printf("\nbufferProx[%d] = %d",proxIndex,buffer[proxIndex]);
	switch(buffer[*tamanho]){
		case HLT:
			printf("\nResultado final:\nAC: %d\nPC: %d",buffer[1],*Pc);
			printf("\n[ ");
			for(int j = 0;j<MAX_SIZE;j++){
				printf("%d, ",buffer[j]);
			}
			printf("]");
			exit(0);
		break;
		case STA:
			// printf(" STA");
			buffer[buffer[proxIndex]] = *Ac;
			(*tamanho)++;
		break;
		case LDA:
			// printf(" LDA");
			*Ac = buffer[buffer[proxIndex]];
			(*tamanho)++;
		break;
		case ADD:
			// printf(" ADD");
			*Ac += buffer[buffer[proxIndex]];
			(*tamanho)++;
		break;
		case OR:
			// printf(" OR");
			*Ac |= buffer[buffer[proxIndex]];
			(*tamanho)++;
		break;
		case AND:
			// printf(" AND");
			*Ac &= buffer[buffer[proxIndex]];
			(*tamanho)++;
		break;
		case NOT:
			// printf(" NOT");
			*Ac = ~*Ac;
		break;
		case JMP:
			// printf(" JMP");
			(*tamanho) = buffer[proxIndex];
		break;
		case JN:
			// printf(" JN");
			if(verificaNegativo(buffer[buffer[proxIndex]]))
				*tamanho = buffer[proxIndex];
		break;
		case JZ:
			// printf(" JZ");
			if(*Ac == 0){
				*tamanho = buffer[proxIndex];
			}
		break;
	}
	(++(*tamanho));
	printf("\n");
}

int binary(FILE *file){
	
	uint8_t buffer [MAX_SIZE];
	uint8_t AC,PC = 0;
	uint8_t i = 0;
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
