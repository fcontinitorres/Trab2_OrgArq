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
int criar_indices(FILE *saida, FILE *ind1, FILE* ind2, FILE* ind3) {

    Registro* reg; // registro a ser lido
    int byte_offset = 0; // byte offset atual
    char c = 'c'; // caracter para percorrer arquivo

    // abre e valida arquivos de índice
    ind1 = fopen(FILE_IND1, "wb+");
    if (!ind1) {
        printf("Erro ao abrir %s\n", FILE_IND1);
        return EXIT_FAILURE;
    }

    ind2 = fopen(FILE_IND2, "wb+");
    if (!ind2) {
        printf("Erro ao abrir %s\n", FILE_IND2);
        return EXIT_FAILURE;
    }

    ind3 = fopen(FILE_IND3, "wb+");
    if (!ind3) {
        printf("Erro ao abrir %s\n", FILE_IND3);
        return EXIT_FAILURE;
    }

    // percorrer arquivo binário
    do {

        // descobre o byte offset atual
        byte_offset = ftell(saida);

        // preparar registro para leitura
        reg = (Registro *) malloc(sizeof(Registro));
        anularCampos(reg);

        // ler campo CNPJ
        fread(&reg->cnpj,sizeof(char),SIZE_CNPJ,saida);

        // indexar CNPJ nos arquivos de índice
        fwrite(reg->cnpj, SIZE_CNPJ, 1, ind1);
        fwrite(reg->cnpj, SIZE_CNPJ, 1, ind2);
        fwrite(reg->cnpj, SIZE_CNPJ, 1, ind3);

        // indexar referencia (byte offeset) nos arquivos de índice
        fwrite(&byte_offset, sizeof(int), 1, ind1);
        fwrite(&byte_offset, sizeof(int), 1, ind2);
        fwrite(&byte_offset, sizeof(int), 1, ind3);

        // pular campos fixos
        fseek(saida, SIZE_DATA + SIZE_DATA + SIZE_CNPJ, SEEK_CUR);

        // ler até próximo registro
        do {
            fread(&c, sizeof(char), 1, saida);
        } while (c != DEL_REG);

    } while (!feof(saida));

    return 1;

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

char* int2string(int val, int base) {

	static char buf[32] = {0};

	int i = 30;

	for(; val && i ; --i, val /= base)

		buf[i] = "0123456789abcdef"[val % base];

	return &buf[i+1];

}
