/*
GRUPO G5
Felipe Scrochio Custodio, 9442688
Felipe Contini Torres, 9253670
Júlia Diniz Ferreira, 9364865
Bruno Henrique Rasteiro, 9292910
*/

#include <locale.h>
#include "registro.h"
#include "indice.h"
#include "menu.h"

int main() {

	int flag; // determina se o programa deve continuar no menu
	int opcao; // opção para o menu, escolhida pelo usuário

	FILE *entrada, *saida1, *saida2, *saida3; // arquivos de dados
	FILE *indice1, *indice2, *indice3; // arquivos de índice primário

    INDICE* indice = NULL; // índice em memória primária

	// abre e verifica se os arquivos foram abertos com sucesso
	if (!abre_saidas(&entrada, &saida1, &saida2, &saida3)) {
		printf("\nErro ao abrir arquivos de saída\n");
		return EXIT_FAILURE;
	}

	// abre e valida arquivos de índice
    if (!abre_indices(&indice1, &indice2, &indice3)) {
		printf("\nErro ao abrir arquivos de índice\n");
		return EXIT_FAILURE;
	}

	printf("\n++++++++++++++++++++++++++++++++++++++++\n");
	printf("+ TRABALHO 2 - ORGANIZAÇÃO DE ARQUIVOS +\n");
	printf("++++++++++++++++++++++++++++++++++++++++\n");

	// gera arquivos de saida (binario)
	csv2Bin(entrada, saida1, saida2, saida3);

	// reseta ponteiro dos arquivos binarios
	fseek(saida1, 0, SEEK_SET);
	fseek(saida2, 0, SEEK_SET);
	fseek(saida3, 0, SEEK_SET);

	// gerar índice primário (em memória primária)
	indice = criar_indices(saida1);

	// fechando arquivo de entrada
	fclose(entrada);

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
				printf("\nOpção inválida.\n");
		}
	}

	// fechando arquivos de saída
	fclose(saida1);
	fclose(saida2);
	fclose(saida3);

	// fechando arquivos de índice
	fclose(indice1);
	fclose(indice2);
	fclose(indice3);

	// TODO - Destruir índices
	// TODO - Destruir qualquer coisa alocada em heap

}
