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

/*	Descrição:
		Lê e retorna string de tamanho 'len'
	Parâmetros:
		int len = tamanho da string a ser lida
*/
char* readStr(int len) {
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

	if (remover(saida1, indice1, strBusca)){
		imprimir_indice(indice1);
		printf("Registro(arquivo1) removido com sucesso.\n");
	}else
		printf("Erro ao remover registro(arquivo1), CNPJ não encontrado.\n");
	
	if (remover(saida2, indice2, strBusca)){
		imprimir_indice(indice2);
		printf("Registro(arquivo2) removido com sucesso.\n");
	}else
		printf("Erro ao remover registro(arquivo2), CNPJ não encontrado.\n");
	
	if (remover(saida3, indice3, strBusca)){
		imprimir_indice(indice3);
		printf("Registro(arquivo3) removido com sucesso.\n");
	}else
		printf("Erro ao remover registro(arquivo3), CNPJ não encontrado.\n");
}

/*	Descrição:
		Realiza as tarefas para executar a terceira opção do menu
	Parâmetros:
		FILE* = arquivos de saída
        INDICE* = índices em memória primária
*/
void opcao3(INDICE* indice1, INDICE* indice2, INDICE* indice3, FILE* saida1, FILE* saida2, FILE* saida3) {
	Registro* reg;
	char* str;
	int op;

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

	printf("\n\nModo de Insercao: \n1 - First Fit\n2 - Best Fit\n3 - Worst Fit\n");
	scanf("%d", &op);


	// inserindo
	switch(op){
		case 1:	if (inserirFirstFit(saida1, indice1, reg)){
				imprimir_indice(indice1);
				printf("Registro inserido(First Fit) com sucesso.\n");
			}else
				printf("Erro ao inserir o registro. Tente novamente.\n");
			break;
		
		case 2: if (insereReg(saida2, indice2, reg, op)){
				imprimir_indice(indice2); 
				printf("Registro inserido(Best Fit) com sucesso.\n");
			}else
				printf("Erro ao inserir o registro. Tente novamento.\n");
			break;

		case 3: if (insereReg(saida3, indice3, reg, op)){
				imprimir_indice(indice3);
				printf("Registro inserido(Worst Fit) com sucesso.\n");
			}else
				printf("Erro ao inserir o registro. Tente novamente.\n");
			break;
	}
}

void opcao4(INDICE* r1, INDICE* r2, INDICE* r3) {

	printf("\nÍNDICE 1\n");
	imprimir_indice(r1);
	printf("\nÍNDICE 2\n");
	imprimir_indice(r2);
	printf("\nÍNDICE 3\n");
	imprimir_indice(r3);

}

void opcao5() {

}
