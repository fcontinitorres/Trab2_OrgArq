/*
GRUPO G5
Felipe Scrochio Custodio, 9442688
Felipe Contini Torres, 9253670
Júlia Diniz Ferreira, 9364865
Bruno Henrique Rasteiro, 9292910
*/

#include "indice.h"

/********************************************//**
 *  Funções para criação e manipulação de índices
 ***********************************************/

/*
    Descrição:
		* Gera arquivo de índice primário baseado nos registros do arquivo 'saida', gravando no arquivo 'indice_primario'
	Parâmetros:
        * FILE* saida = arquivo binário com os
        * dados obtidos a partir da leitura do csv
        * FILE* indice_primario = arquivo de índice primário a ser gerado
*/
int criar_indices(FILE *saida, FILE *ind1, FILE* ind2, FILE* ind3) {

    int i;
    // inicializar registro
    Registro* reg; // registro a ser lido
    reg = (Registro *)malloc(sizeof(Registro));
    int byte_offset = 0; // byte offset atual
    char c = 'c'; // caracter para percorrer arquivo

    // inicializar índice
    INDICE* indice = (INDICE*)malloc(sizeof(INDICE));
    indice->lista = (NO**)malloc(sizeof(NO**));
    indice->tamanho = 0;

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

        // armazena o byte offset atual
        byte_offset = ftell(saida);

        // preparar registro para leitura
        anularCampos(reg);

        // ler campo CNPJ
        fread(&reg->cnpj,sizeof(char),SIZE_CNPJ,saida);

        // salvar em novo nó na lista de índices
        indice->lista = (NO**)realloc(indice->lista, sizeof(NO*) * indice->tamanho + 1);
        indice->lista[indice->tamanho] = (NO*)malloc(sizeof(NO));
        indice->lista[indice->tamanho]->referencia = 0;

        // copiar CNPJ do registro para o novo nó de índice
        for (i = 0; i < SIZE_CNPJ + 1; i++) {
            indice->lista[indice->tamanho]->chave[i] = reg->cnpj[i];
        }

        // salvar byte offset como referência desse CNPJ
        indice->lista[indice->tamanho]->referencia = byte_offset;

        // aumentar contador da lista de índices
        indice->tamanho += 1;

        // pular campos fixos
        fseek(saida, SIZE_DATA + SIZE_DATA + SIZE_CNPJ, SEEK_CUR);

        // ler até próximo registro
        do {
            fread(&c, sizeof(char), 1, saida);
        } while (c != DEL_REG);

    } while (!feof(saida));
        // zerar nova chave

    // ordenar índices
    

    // escrever índices nos arquivos

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

NO* copiar_no(NO* a, NO* b) {
    // copia dados do nó B para nó A
    strcpy(a->chave, b->chave);
    a->referencia = b->referencia;
    return a;
}

void atualizar_indice(NO* indice) {




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

    char cnpj[SIZE_CNPJ + 1];
    char c = '@';
    char flag = 0;
    do {
        // ler campo CNPJ
        fread(&cnpj,sizeof(char),SIZE_CNPJ,indice);
        // comparar com conteúdo buscado
        // se encontrado, retornar byte offset
        // se não encontrado, pular byte offset
        fseek(indice, sizeof(int), SEEK_CUR);
    } while (!feof(indice) || !flag);

    if (!flag) printf("Campo não encontrado!\n");

}

/*
    Descrição:
		* Pesquisa índice pela chave de referência
	Parâmetros:
        * indice = arquivo de índice primário onde será feita
        * a busca
        * chave = chave de referência a ser buscada
*/
void pesquisa_indice_ref(FILE* indice, int chave) {

    char cnpj[SIZE_CNPJ];
    int referencia;
    char c = '@';
    do {
        // pular campo CNPJ
        fread(&cnpj,SIZE_CNPJ,1,indice);
        // ler campo referência
        fread(&referencia,sizeof(int),1,indice);
        // comparar com conteúdo buscado
        if (referencia == chave) {
            //return cnpj;
        }
        // se encontrado, retornar CNPJ
    } while (!feof(indice));

    printf("Campo não encontrado!\n");

}

/*
    Descrição:
		* Destrói arquivo de índices ao final da execução
	Parâmetros:
        * indice = arquivo a ser destruído
*/
void destruir_indice(FILE* indice) {


}
