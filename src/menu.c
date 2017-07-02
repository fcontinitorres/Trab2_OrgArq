/*
GRUPO G5
Felipe Scrochio Custodio, 9442688
Felipe Contini Torres, 9253670
Júlia Diniz Ferreira, 9364865
Bruno Henrique Rasteiro, 9292910
*/

#include <stdio.h>
#include <stdlib.h>
#include "menu.h"
#include "indice.h"
#include "estatistica.h"


char* readStr(int len){
	char* str;
	int i, count;

	str = (char *) malloc(len * sizeof(char));

	fgets(str, len, stdin);

	for (i = 0; i < len; i++)
		if (str[i] == '\n')
			break;

	str[i] = '\0';
	
	str = (char *) realloc(str, (i) * sizeof(char));

	return(str);
}

/*	Descrição:
		Abre os arquivos de entrada e saída do programa
	Parâmetros:
		entrada = arquivo de entrada
		saida1 = arquivo de saída
		saida2 = arquivo de saída
		saida3 = arquivo de saída
	Retorno:
		Retorna um int que indica o sucesso da função. Os possíves valores são:
		1 -> Arquivos aberto com sucesso
		EXIT_FAILURE -> Falha ao abrir arquivos
*/
int abre_saidas(FILE **entrada, FILE **saida1, FILE **saida2, FILE **saida3) {

	// abrindo arquivos
    *entrada  = fopen(FILE_IN, "r");
    *saida1 = fopen(FILE_OUT1, "wb+");
    *saida2 = fopen(FILE_OUT2, "wb+");
    *saida3 = fopen(FILE_OUT3, "wb+");

    if (*entrada == NULL) {
    	printf("Erro ao abrir: %s\n", FILE_IN);
    	return EXIT_FAILURE;
    }

    if (*saida1 == NULL) {
    	printf("Erro ao abrir: %s\n", FILE_OUT1);
    	return EXIT_FAILURE;
    }

    if (*saida2 == NULL) {
    	printf("Erro ao abrir: %s\n", FILE_OUT2);
    	return EXIT_FAILURE;
    }

    if (*saida3 == NULL) {
    	printf("Erro ao abrir: %s\n", FILE_OUT3);
    	return EXIT_FAILURE;
    }

    return(1);
}

/*	Descrição:
		Realiza as tarefas para executar a primeira opção do menu
	Parâmetros:
		file = arquivo de saida (binário)
*/
void opcao1(FILE *file) {
	// lista todos os dados
	listarBinario(file);
	// reseta ponteiro do arquivo
	fseek(file, 0, SEEK_SET);


/* Eu (bruno) estava usando esse print do arquivo todo para testar a inserção(FF) e remoção

	char c;
	int size;
	long int ref;

	fseek(file, 0, SEEK_SET);

	fread(&ref,  sizeof(long int), 1, file);
	printf("HEAD = %ld\n", ref);

	do {
		c = fgetc(file);
		if (c != EOF)
			printf("%c", c);

		if (c == '@'){
			fread(&size, sizeof(int), 1, file);
			fread(&ref,  sizeof(long int), 1, file);
			printf("'%d'%ld'", size, ref);	
		}

	} while (c != EOF);
*/
}

/*	Descrição:
		Realiza as tarefas para executar a segunda opção do menu
	Parâmetros:
		file = arquivo de saida (binário)
*/
void opcao2(INDICE* indice1, INDICE* indice2, INDICE* indice3, FILE* saida1, FILE* saida2, FILE* saida3) {
	char *strBusca; // valor que a busca irá usar para comparar

	// espera o valor da busca
	printf("Informe o CNPJ a ser buscado: ");
	strBusca = readStr(SIZE_CNPJ + 1);

	if (remover(saida1, indice1, strBusca))
		printf("Registro removido com sucesso.\n");
	else
		printf("Erro ao remover registro, CNPJ não encontrado.\n");

/*
	// busca
	reg = buscaCampoCNPJ(file, strBusca);

	if (reg != NULL) {
		printf("Registro Localizado\n");
		printRegistro(reg);
		free(reg->razSoc);
		free(reg->nomeFant);
		free(reg->motCanc);
		free(reg->nomeEmp);
	}
	else
		printf("Não foi possível localizar o registro.\n");

	// reseta ponteiro do arquivo
	fseek(file, 0, SEEK_SET);
	free(reg);
	free(strBusca);
*/
}

void opcao3(INDICE* indice1, INDICE* indice2, INDICE* indice3, FILE* saida1, FILE* saida2, FILE* saida3) {
	Registro* reg;
	char* str;

	reg = (Registro *) malloc(sizeof(Registro));

	// coletando dados do registro
	printf("Informe o CNPJ: ");
	str = readStr(SIZE_CNPJ + 1);
	strcpy(reg->cnpj, str);
	getchar();
	free(str);
	
	printf("Informe a Data de Registro: ");
	str = readStr(SIZE_DATA + 1);
	strcpy(reg->dtReg, str);
	getchar();
	free(str);

	printf("Informe a Data de Cancelamento: ");
	str = readStr(SIZE_DATA + 1);
	strcpy(reg->dtCanc, str);
	getchar();
	free(str);

	printf("Informe o CNPJ da empresa auditora: ");
	str = readStr(SIZE_CNPJ + 1);
	strcpy(reg->cnpjAud, str);
	getchar();
	free(str);

	printf("Informe a razao social: ");
	reg->razSoc = readStr(1000);

	printf("Informe o nome fantasia: ");
	reg->nomeFant = readStr(1000);

	printf("Informe o motivo de cancelamento: ");
	reg->motCanc = readStr(1000);

	printf("Informe o nome da empresa auditora: ");
	reg->nomeEmp = readStr(1000);

	// inserindo
	if (inserirFF(saida1, indice1, reg))
		printf("Rgistro inserido com sucesso\n");
	else
		printf("Erro ao inserir registro. Tente novamente.\n");
}

void opcao4(INDICE* indice1, INDICE* indice2, INDICE* indice3) {
	indices_estat(indice1, indice2, indice3);
}

void opcao5(FILE *file1, FILE *file2, FILE *file3) {
	printf("Lista de removidos do primeiro arquivo\n");
	getchar();
	listarRemovidos(file1);
	printf("Lista de removidos do segundo arquivo\n");
	getchar();
	listarRemovidos(file2);
	printf("Lista de removidos do terceiro arquivo\n");
	getchar();
	listarRemovidos(file3);
}


