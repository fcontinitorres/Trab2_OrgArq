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
 		* Abre e valida arquivos de índice
 	Parâmetros:
         * FILE* ponteiros para arquivos de índice binários
 */
int abre_indices(FILE** indice1, FILE** indice2, FILE** indice3) {
    // abre e valida arquivos de índice
    *indice1 = fopen(FILE_IND1, "wb+");
    if (!(*indice1)) {
        printf("Erro ao abrir %s\n", FILE_IND1);
        return EXIT_FAILURE;
    }

    *indice2 = fopen(FILE_IND2, "wb+");
    if (!(*indice2)) {
        printf("Erro ao abrir %s\n", FILE_IND2);
        return EXIT_FAILURE;
    }

    *indice3 = fopen(FILE_IND3, "wb+");
    if (!(*indice3)) {
        printf("Erro ao abrir %s\n", FILE_IND3);
        return EXIT_FAILURE;
    }
    return 1;
}


/*
    Descrição:
		* Gera índice em memória primária a partir de um
		* armazenado em memória secundária
	Parâmetros:
        * FILE* arquivo = arquivo contendo índice primário
        * INDICE* indice = índice a ser carregado em memória primária
*/
INDICE* ler_indice(FILE* arquivo, INDICE* indice) {

    // inicializa valores para leitura
    char* CNPJ = (char*)malloc(sizeof(char) * SIZE_CNPJ + 1);
    int referencia = 0;

    // inicializa índice em memória primária
    indice = (INDICE*)malloc(sizeof(INDICE));
    indice->lista = (NO**)malloc(sizeof(NO**));
    indice->tamanho = 0;

    // leitura do arquivo
    do {

        // ler CNPJ
        fread(CNPJ, SIZE_CNPJ, 1, arquivo);
        // ler referencia
        fread(&referencia, sizeof(int), 1, arquivo);

        indice = inserir_indice(indice, CNPJ, referencia);

    } while(!feof(arquivo));

    return indice;

}

/*
    Descrição:
		* Gera arquivo de índice em memória secundária
		* a partir de um índice em memória primária
	Parâmetros:
        * FILE* arquivo = arquivo que receberá índice
        * INDICE* indice = índice que está em memória primária
*/
void escrever_indice(INDICE* indice, FILE* arquivo) {

    int i;
    for (i = 0; i < indice->tamanho; i++) {
        // escrever CNPJ
        fwrite(indice->lista[i]->chave, SIZE_CNPJ, 1, arquivo);
        // escrever referencia
        fwrite(&indice->lista[i]->referencia, sizeof(int), 1, arquivo);
    }
}

/*
    Descrição:
		* Gera índice primário baseado nos registros do arquivo 'saida', gravando em memória primária
	Parâmetros:
        * FILE* saida = arquivo binário com os
        * dados obtidos a partir da leitura do csv
*/
INDICE* criar_indices(FILE *saida) {

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
        indice->lista[indice->tamanho]->chave[SIZE_CNPJ] = '\0';

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

    // ordenar índices
    indice = atualizar_indice(indice);

    if (!indice) {
        printf("Algum erro ocorreu com o índice em memória primária\n");
        exit(0);
    }

    // retornar índice (em memória primária)
    return indice;

}

/*
    Descrição:
		* Insere um novo valor no arquivo de índices,
		* depois reordenando-o.
	Parâmetros:
        *
*/
INDICE* inserir_indice(INDICE* indice, char* CNPJ, int referencia) {

    int i;

    // salvar em novo nó na lista de índices
    indice->lista = (NO**)realloc(indice->lista, sizeof(NO*) * indice->tamanho + 1);
    indice->lista[indice->tamanho] = (NO*)malloc(sizeof(NO));

    // copiar CNPJ para o novo nó de índice
    for (i = 0; i < SIZE_CNPJ + 1; i++) {
        indice->lista[indice->tamanho]->chave[i] = CNPJ[i];
    }
    indice->lista[indice->tamanho]->chave[SIZE_CNPJ] = '\0';

    // salvar referência desse CNPJ
    indice->lista[indice->tamanho]->referencia = referencia;

    // aumentar contador da lista de índices
    indice->tamanho += 1;

    // reordenar (insertion sort rápido para lista quase ordenada)
    indice = atualizar_indice(indice);

    return indice;
}

/*
    Descrição:

	Parâmetros:

*/
int remover(FILE* file, INDICE* indice, char* chave){
    int referencia;

    // pesquisa referência do registro no arquivo de índice
    referencia = pesquisa_indice(indice, chave);

    // se o registro existe
    if (referencia != -1) {

        // remove lógicamente do arquivo de dados
        remover_dado(file, referencia);

        // remove fisicamente do índice em memória ram
        remover_indice(indice, chave);

        return(1);
    }

    return(0);
}

/*
    Descrição:
		* Remove um índice do índice em RAM,
		* reorganizando em seguida.
	Parâmetros:
        * indice = ponteiro para o índice em RAM
        * chave = chave (string) de busca para a remoção
*/
int remover_indice(INDICE* indice, char* chave) {
    int i;
    int k;
    int cmp;
    int removeu = 0;

    for (i=0; i < indice->tamanho; i++){
        cmp = strcmp(chave, indice->lista[i]->chave);

        if (cmp == 0){
            // removendo indice
            free(indice->lista[k]);

            // atualizando vetor de indices
            for (k=i+1; k < indice->tamanho; k++)
                // TODO - acho melhor usar copiar_no
                indice->lista[k-1] = indice->lista[k];

            // atualizando tamanho do indice
            indice->tamanho--;

            removeu = 1;
            break;
        }
        else if (cmp > 0)
            break;
    }

    return(removeu);
}

/*
    Descrição:
        * Remove um ,
        * reorganizando em seguida.
    Parâmetros:

*/
void remover_dado(FILE* file, int referencia){
    char exc_log = EXC_LOG;

    // fseek até o registro a ser deletado
    fseek (file, referencia, SEEK_SET);

    // grava o caracter de exclusão lógica
    fwrite(&exc_log, sizeof(char), sizeof(exc_log), file);
}

/*
    Descrição:
		* Copia o conteúdo do nó B para o nó A.
	Parâmetros:
        * a = Nó que recebe conteúdo
        * b = Nó que provém conteúdo
*/
NO* copiar_no(NO* a, NO* b) {
    int i;
    // copia dados do nó B para nó A
    for (i = 0; i < SIZE_CNPJ + 1; i++) {
        a->chave[i] = b->chave[i];
    }
    a->chave[SIZE_CNPJ + 1] = '\0';
    a->referencia = b->referencia;
    return a;
}

/*
    Descrição:
		* Converte string CNPJ para número
	Parâmetros:
        * char* CNPJ - string a ser convertida
*/
long int converter_CNPJ(char* CNPJ) {
    int i;
    long int convertido = 0;
    for (i = 0; i < SIZE_CNPJ - 1; i++) {
        if (isdigit(CNPJ[i])) {
            convertido = (convertido * 10) + (CNPJ[i] - '0');
        }
    }
    return convertido;
}

/*
    Descrição:
		* Reordena arquivo de índices após inserção/remoção.
	Parâmetros:
        * indice = arquivo de índice primário a ser reorganizado
*/
INDICE* atualizar_indice(INDICE* indice) {
    int i, j;

    NO* atual = (NO*)malloc(sizeof(NO));

    for (i = 1; i < indice->tamanho; i++) {
        atual = copiar_no(atual, indice->lista[i]);
        j = i - 1;
        while ((j >= 0) && (converter_CNPJ(indice->lista[j]->chave) > converter_CNPJ(atual->chave))) {
            indice->lista[j+1] = copiar_no(indice->lista[j+1], indice->lista[j]);
            j--;
        }
        indice->lista[j+1] = copiar_no(indice->lista[j+1], atual);
    }

    imprimir_indice(indice);
    return indice;
}

/*
    Descrição:
		* Imprime índice de forma tabelada
	Parâmetros:
        * indice = índice em memória primária que será exibido
*/
void imprimir_indice(INDICE* indice) {
    int i, j;
    printf("[ÍNDICE]\t[CNPJ]\t\t\t[BYTE OFFSET]\n");
    for (i = 0; i < indice->tamanho; i++) {
        printf("ÍNDICE [%d]\t", i);
        // imprimir CNPJ
        for (j = 0; j < SIZE_CNPJ; j++) {
            printf("%c", indice->lista[i]->chave[j]);
        }
        // imprimir referência
        printf("\t%d\n", indice->lista[i]->referencia);
    }
}

int pesquisa_indice(INDICE* indice, char* chave){
    int i;
    int cmp;
    int referencia = -1;

    for (i=0; i < indice->tamanho; i++){
        cmp = strcmp(chave, indice->lista[i]->chave);

        if (cmp == 0){
            // salvando referencia para retornar
            referencia = indice->lista[i]->referencia;
            break;
        }
        else if (cmp > 0)
            break;
    }

    return(referencia);
}
/*
    Descrição:
		* Pesquisa índice pela chave de busca, retornando
		* a referencia (byte offset) do registro
	Parâmetros:
        * indice = arquivo de índice onde será feita a busca
        * chave = chave a ser buscada
*/
int pesquisa_indice_chave(FILE* indice, char* chave) {

    char cnpj[SIZE_CNPJ + 1];
    int bos;
    int cmp;

    do {
        // ler campo CNPJ
        fread(cnpj, sizeof(char), SIZE_CNPJ, indice);

        // comparar com conteúdo buscado
        cmp = strcmp(chave, cnpj);

        // se encontrou
        if (cmp == 0){
            // ler campo CNPJ
            fread(&bos, sizeof(int), 1, indice);
            // retornar byte offset
            return(bos);
        }

        // se não encontrado, pular byte offset
        fseek(indice, sizeof(int), SEEK_CUR);
    } while (!feof(indice) || cmp < 0);

    if (cmp > 0)
        return(-1);
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
		* Destrói índice da memória primária
	Parâmetros:
        * indice = a ser destruído
*/
void destruir_indice(INDICE** indice) {
    int i;
    for (i = 0; i < (*indice)->tamanho; i++) {
        free((*indice)->lista[i]);
        (*indice)->lista[i] = NULL;
    }
    free((*indice)->lista);
    (*indice)->lista = NULL;

    free((*indice));
    (*indice) = NULL;

}
