/*
GRUPO G5
Felipe Scrochio Custodio, 9442688
Felipe Contini Torres, 9253670
Júlia Diniz Ferreira, 9364865
Bruno Henrique Rasteiro, 9292910
*/
#include "estatistica.h"

int quant_entradas(Registro *reg){

	int count = 0;
	if(reg->cnpj[0] == 'n') count++;
	if(reg->dtReg[0] == 'n') count++;
	if(reg->dtCanc[0] == 'n') count++;
	if(reg->cnpjAud[0] == 'n') count++;
	if(reg->razSoc == NULL) count++;
	if(reg->nomeFant == NULL) count++;
	if(reg->motCanc == NULL) count++;
	if(reg->nomeEmp == NULL) count++;

	return count;
}

void indices_estat(INDICE* indiceR1, INDICE* indiceR2, INDICE* indiceR3){

	//int quantI1 = indiceR1->tamanho, quantI2 = indiceR1->tamanho, quantI3 = indiceR1->tamanho;
	//printf("Indice // Quantidade de entradas\n");
	//printf("   1   //            %d         \n", indiceR1->tamanho);
	//printf("   2   //            %d         \n", indiceR2->tamanho);
	//printf("   3   //            %d         \n", indiceR3->tamanho);
	char last_chave[SIZE_CNPJ + 1];
	int count, i;

	printf("Arquivo de Indice 1\n");
	printf("Indice Primario \t// Quantidade\n");
	strcpy(last_chave,indiceR1->lista[0]->chave);
	count = 1;
	for(i=1;i<indiceR1->tamanho;i++){
		if(strcmp(indiceR1->lista[i]->chave, last_chave) == 0){
			count++;
		}
		else{
			printf("%s \t//     %d\n",last_chave, count);
			count = 1;
			strcpy(last_chave,indiceR1->lista[i]->chave);
		}
	}
	printf("%s \t// %d\n",last_chave, count);
	/*
	printf("Arquivo de Indice 2\n");
	printf("Indice Primario \t// Quantidade\n");
	strcpy(last_chave,indiceR2->lista[0]->chave);
	count = 1;
	for(i=1;i<indiceR2->tamanho;i++){
		if(strcmp(indiceR2->lista[i]->chave, last_chave) == 0){
			count++;
		}
		else{
			printf("%s \t//     %d\n",last_chave, count);
			count = 1;
			strcpy(last_chave,indiceR2->lista[i]->chave);
		}
	}
	printf("%s \t// %d\n",last_chave, count);
	*/
	/*
	printf("Arquivo de Indice 3\n");
	printf("Indice Primario \t// Quantidade\n");
	strcpy(last_chave,indiceR3->lista[0]->chave);
	count = 1;
	for(i=1;i<indiceR3->tamanho;i++){
		if(strcmp(indiceR3->lista[i]->chave, last_chave) == 0){
			count++;
		}
		else{
			printf("%s \t//     %d\n",last_chave, count);
			count = 1;
			strcpy(last_chave,indiceR3->lista[i]->chave);
		}
	}
	printf("%s \t//     %d\n",last_chave, count);
	*/

	printf("\nArquivo \t\t//CNPJ \t// Byte Offset\n");
	for(i=0;i<indiceR1->tamanho;i++){
		printf("Registro %d\n", i+1);
		printf("Arquivo de Indice 1 \t// %s \t// %ld\n",indiceR1->lista[i]->chave, indiceR1->lista[i]->referencia);
		printf("Arquivo de Indice 2 \t// %s \t// %ld\n",indiceR2->lista[i]->chave, indiceR2->lista[i]->referencia);
		printf("Arquivo de Indice 3 \t// %s \t// %ld\n",indiceR3->lista[i]->chave, indiceR3->lista[i]->referencia);
		printf("PRESS ENTER\n");
		scanf("%*c");
	}
}