/*
GRUPO G5
Felipe Scrochio Custodio, 9442688
Felipe Contini Torres, 9253670
Júlia Diniz Ferreira, 9364865
Bruno Henrique Rasteiro, 9292910
*/

#include "registro.h"
#include "menu.h"

int main() {
	int flag;
	int opcao;
	FILE *fileIn, *fileOut1, *fileOut2, *fileOut3;

	// verifica se os arquivos foram abertos com sucesso
	if (!validaArquivos(&fileIn, &fileOut1, &fileOut2, &fileOut3)) {
		return(0);
	}

	printf("\n++++++++++++++++++++++++++++++++++++++++\n");
	printf("+ TRABALHO 2 - ORGANIZAÇÃO DE ARQUIVOS +\n");
	printf("++++++++++++++++++++++++++++++++++++++++\n");
	printf("Organizando Arquivos com Delimitadores entre registros\n");

	// gera arquivos de saida (binario)
	csv2Bin(fileIn, fileOut1, fileOut2, fileOut3);

	// fechando arquivo de entrada
	fclose(fileIn);

	// reseta ponteiro dos arquivos binarios
	fseek(fileOut1, 0, SEEK_SET);
	fseek(fileOut2, 0, SEEK_SET);
	fseek(fileOut3, 0, SEEK_SET);

	flag = 1;
	while(flag) {

    	printf("\n++++++++\n");
    	printf("+ MENU +\n");
    	printf("++++++++\n");
    	printf("1 - Listar todos os registros\n");
		printf("2 - Buscar um registro pelo CNPJ\n");
		printf("3 - Sair\n");
		printf("Escolha uma opção: ");
		scanf("%d", &opcao);
		getchar();

		switch(opcao) {
			case 1:
				opcao1(fileOut1);
			break;

			case 2:
				opcao2(fileOut1);
			break;

			case 3:
				flag = 0;
			break;

			default:
				printf("\nOpcao invalida.\n");
		}
	}

	// fechando arquivos
	fclose(fileOut1);
	fclose(fileOut2);
	fclose(fileOut3);
}
