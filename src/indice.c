/*
GRUPO G5
Felipe Scrochio Custodio, 9442688
Felipe Contini Torres, 9253670
Júlia Diniz Ferreira, 9364865
Bruno Henrique Rasteiro, 9292910
*/

#include "indice.h"

/*
    Descrição:
		* Gera arquivo de índice primário baseado nos registros do arquivo 'saida', gravando no arquivo 'indice_primario'
	Parâmetros:
        * FILE* saida = arquivo binário com os
        * dados obtidos a partir da leitura do csv
        * FILE* indice_primario = arquivo de índice primário a ser gerado
*/
void criar_indice(FILE *saida, FILE *indice_primario) {

    NODE* indice = (NODE*)malloc(sizeof(NODE));

    // percorrer arquivo binário


    // gravar índice primário em arquivo

}

/*
    Descrição:
		* Insere um novo valor no arquivo de índices,
		* depois reordenando-o.
	Parâmetros:
        *
*/
void inserir_indice() {

}

/*
    Descrição:
		* Remove um índice de um arquivo de índices,
		* reorganizando em seguida.
	Parâmetros:
        *
*/
void remover_indice() {

}

/*
    Descrição:
		* Reordena arquivo de índices após inserção/remoção.
	Parâmetros:
        * indice = arquivo de índice primário a ser reorganizado
*/
void atualizar_indice(FILE* indice) {

}

/*
    Descrição:
		* Pesquisa índice pela chave de busca, retornando
		* o valor de referência
	Parâmetros:
        * indice = arquivo de índice onde será feita a busca
        * chave = chave a ser buscada
*/
void pesquisa_indice_chave(FILE* indice, char* chave) {

}

/*
    Descrição:
		* Pesquisa índice pela chave de referência
	Parâmetros:
        * indice = arquivo de índice primário onde será feita
        * a busca
        * chave = chave de referência a ser buscada
*/
void pesquisa_indice_ref(FILE* indice, char* chave) {

}

/*
    Descrição:
		* Destrói arquivo de índices ao final da execução
	Parâmetros:
        * indice = arquivo a ser destruído
*/
void destruir_indice(FILE* indice) {

}
