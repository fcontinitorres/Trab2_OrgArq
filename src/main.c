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
	FILE *indiceF1, *indiceF2, *indiceF3; // arquivos de índice primário

	// índice em memória primária
    INDICE *indiceR1 = NULL, *indiceR2 = NULL, *indiceR3 = NULL;

	// abre e verifica se os arquivos foram abertos com sucesso
	if (!abre_saidas(&entrada, &saida1, &saida2, &saida3)) {
		printf("\nErro ao abrir arquivos de saída\n");
		return EXIT_FAILURE;
	}

	// abre e valida arquivos de índice
    if (!abre_indices(&indiceF1, &indiceF2, &indiceF3)) {
		printf("\nErro ao abrir arquivos de índice\n");
		return EXIT_FAILURE;
	}

	printf("\n++++++++++++++++++++++++++++++++++++++++\n");
	printf("+ TRABALHO 2 - ORGANIZAÇÃO DE ARQUIVOS +\n");
	printf("++++++++++++++++++++++++++++++++++++++++\n");

	// gera arquivos de saida (binario)
	csv2Bin(entrada, saida1, saida2, saida3);

	// fechando arquivo de entrada
	fclose(entrada);

	// gerar índice primário (em memória primária)
	indiceR1 = criar_indices(saida1);
	indiceR2 = criar_indices(saida2);
	indiceR3 = criar_indices(saida3);

	// interface
	flag = 1;
	while(flag) {
		printf("\n");
		imprimir_indice(indiceR1);
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
				opcao2(indiceR1, indiceR2, indiceR3, saida1, saida2, saida3);
				break;

			case 3:
				// TODO - inserção de registro
				opcao3(indiceR1, indiceR2, indiceR3, saida1, saida2, saida3);
				break;

			case 4:
				// TODO - estatísticas índices
				opcao4(indiceR1, indiceR2, indiceR3);
				break;

			case 5:
				opcao5(saida1, saida2, saida3);
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
	fclose(indiceF1);
	fclose(indiceF2);
	fclose(indiceF3);

	// TODO - Destruir índices
	// TODO - Destruir qualquer coisa alocada em heap

}
