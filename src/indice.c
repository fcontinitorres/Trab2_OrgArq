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
        // zerar nova chave

    // ordenar índices
    indice = atualizar_indice(indice);

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


int remover(FILE* file, INDICE* indice, char* chave){
    int referencia;

    // pesquisa referência do registro no arquivo de índice
    referencia = pesquisa_indice(indice, chave);

    // se o registro 
    if (referencia != -1){
        
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
        while ((j > 0) && (atoi(indice->lista[j]->chave) > atoi(atual->chave))) {
            indice->lista[j+1] = copiar_no(indice->lista[j+1], indice->lista[j]);
            j--;
        }
        indice->lista[j+1] = copiar_no(indice->lista[j+1], atual);
    }
    
    imprimir_indice(indice);
    return indice;
}

void imprimir_indice(INDICE* indice) {
    int i;
    for (i = 0; i < indice->tamanho; i++) {
        printf("Índice [%d]\t%s\t%d\n", i, indice->lista[i]->chave, indice->lista[i]->referencia);
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
