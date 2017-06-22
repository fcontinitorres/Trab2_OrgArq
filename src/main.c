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
	FILE *entrada, *saida1, *saida2, *saida3;

	// verifica se os arquivos foram abertos com sucesso
	if (!validaArquivos(&entrada, &saida1, &saida2, &saida3)) {
		return(0);
	}

	printf("\n++++++++++++++++++++++++++++++++++++++++\n");
	printf("+ TRABALHO 2 - ORGANIZAÇÃO DE ARQUIVOS +\n");
	printf("++++++++++++++++++++++++++++++++++++++++\n");

	// gera arquivos de saida (binario)
	csv2Bin(entrada, saida1, saida2, saida3);

	// fechando arquivo de entrada
	fclose(entrada);

	// reseta ponteiro dos arquivos binarios
	fseek(saida1, 0, SEEK_SET);
	fseek(saida2, 0, SEEK_SET);
	fseek(saida3, 0, SEEK_SET);

	// interface
	flag = 1;
	while(flag) {

    	printf("\n++++++++++++++++++++++++++++++++++++++++\n");
		printf("+                 MENU                 +\n");
		printf("++++++++++++++++++++++++++++++++++++++++\n");
    	printf("1 - Listar todos os registros\n");
		printf("2 - Remover registro pelo CNPJ\n");
		printf("3 - Inserir registro adicional\n");
		printf("4 - Estatísticas sobre arquivos de índice\n");
		printf("5 - Estatísticas sobre os registros removidos\n");
		printf("6 - Sair\n");
		printf("Escolha uma opção: ");
		scanf("%d", &opcao);
		getchar();

		switch(opcao) {
			case 1:
				opcao1(saida1);
				break;

			case 2:
				opcao2(saida1);
				break;

			case 3:
				// TODO - inserção de registro
				opcao3();
				break;

			case 4:
				// TODO - estatísticas índices
				opcao4();
				break;

			case 5:
				// TODO - estatísticas remoções
				opcao5();
				break;

			case 6:
				printf("Saindo\n");
				flag = 0;
				break;

			default:
				printf("\nOpcao invalida.\n");
		}
	}

	// fechando arquivos
	fclose(saida1);
	fclose(saida2);
	fclose(saida3);
}
