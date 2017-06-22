/*
GRUPO G5
Felipe Scrochio Custodio, 9442688
Felipe Contini Torres, 9253670
Júlia Diniz Ferreira, 9364865
Bruno Henrique Rasteiro, 9292910
*/

#include "menu.h"

/*	Descrição:
		Abre os arquivos de entrada e saída do programa
	Parêmetros:
		fileIn = arquivo de entrada
		fileOut1 = arquivo de saída
		fileOut2 = arquivo de saída
		fileOut3 = arquivo de saída
	Retorno:
		Retorna um int que indica o sucesso da função. Os possíves valores são:
		1 -> Arquivos aberto com sucesso
		2 -> Falha ao abrir arquivos */
int validaArquivos(FILE **fileIn, FILE **fileOut1, FILE **fileOut2, FILE **fileOut3) {

	// abrindo arquivos
    *fileIn  = fopen(FILE_IN, "r");
    *fileOut1 = fopen(FILE_OUT1, "wb+");
    *fileOut2 = fopen(FILE_OUT2, "wb+");
    *fileOut3 = fopen(FILE_OUT3, "wb+");

    if (*fileIn == NULL) {
    	printf("Erro ao abrir: %s\n", FILE_IN);
    	return(0);
    }

    if (*fileOut1 == NULL) {
    	printf("Erro ao abrir: %s\n", FILE_OUT1);
    	return(0);
    }

    if (*fileOut2 == NULL) {
    	printf("Erro ao abrir: %s\n", FILE_OUT2);
    	return(0);
    }

    if (*fileOut3 == NULL) {
    	printf("Erro ao abrir: %s\n", FILE_OUT3);
    	return(0);
    }

    return(1);
}

/*	Descrição:
		Realiza as tarefas para executar a primeira opção do menu
	Parêmetros:
		file = arquivo de saida (binário) */
void opcao1(FILE *file) {
	// lista todos os dados
	listBin(file);

	// reseta ponteiro do arquivo
	fseek(file, 0, SEEK_SET);
}

/*	Descrição:
		Realiza as tarefas para executar a segunda opção do menu
	Parêmetros:
		file = arquivo de saida (binário) */
void opcao2(FILE *file) {
	char *strBusca; // valor que a busca irá usar para comparar
	Registro *reg; // registro resultante da busca

	// espera o valor da busca
	printf("Informe o cnpj a ser buscado: ");
	strBusca = NULL;
	scanf("%ms", &strBusca);

	// busca
	reg = buscaCampoCNPJ(file, strBusca);

	if (reg != NULL) {
		printf("Registro Localizado\n");
		printReg(reg);
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
}
