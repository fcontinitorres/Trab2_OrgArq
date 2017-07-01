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

int remover(FILE* file, INDICE* indice, char* chave){
    long int referencia;

    // pesquisa referência do registro no arquivo de índice
    referencia = pesquisa_indice(indice, chave);

    // se o registro existe
    if (referencia != -1) {

        // remove lógicamente do arquivo de dados
        _remover_dado(file, referencia);

        // remove fisicamente do índice em memória ram
        _remover_indice(indice, chave);

        return(1);
    }

    return(0);
}

/*
    Descrição:
        * Remove um ,
        * reorganizando em seguida.
    Parâmetros:
*/
void _remover_dado(FILE* file, long int referencia){
    char exc_log = EXC_LOG; 
    long int head;
    int sizeReg;
    char c;

    // fseek para o inicio do arquivo
    fseek (file, 0, SEEK_SET);

    // le a cabeça da lista de remoção (byte offset do registro topo da lista)
    fread(&head, sizeof(long int), 1, file);

    // fseek para o inicio do arquivo
    fseek (file, 0, SEEK_SET);

    // atualiza a lista de remoção
    fwrite(&referencia, sizeof(long int), 1, file);

    // soma o tamanho dos campos fixos
    sizeReg = SIZE_CNPJ*2 + SIZE_DATA*2;

    // fseek até o inicio dos campos variaveis do registro a ser deletado
    fseek (file, referencia + sizeReg, SEEK_SET);

    // descobre o tamanho do registro
    do {
        fread(&c, sizeof(char), 1, file);
        if (c != DEL_REG)
            sizeReg++;
    } while (c != DEL_REG);
    
    // fseek até o inicio do registro a ser deletado
    fseek (file, referencia, SEEK_SET);

    // grava o caracter de exclusão lógica
    fwrite(&exc_log, sizeof(char), 1, file);

    // grava o tamanho do registro
    fwrite(&sizeReg, sizeof(int), 1, file);

    // grava o byte ofsset do próximo registro removido (antiga cabeça da lista)
    fwrite(&head, sizeof(long int), 1, file);
}

/*
    Descrição:
        * Remove um índice do índice em RAM,
        * reorganizando em seguida.
    Parâmetros:
        * indice = ponteiro para o índice em RAM
        * chave = chave (string) de busca para a remoção
*/
int _remover_indice(INDICE* indice, char* chave) {
    int i;
    int k;
    int cmp;
    int removeu = 0;

    for (i=0; i < indice->tamanho; i++){
        cmp = strcmp(chave, indice->lista[i]->chave);

        if (cmp == 0){

            // atualizando vetor de indices
            for (k=i+1; k < indice->tamanho; k++)
                //indice->lista[k-1] = indice->lista[k];
                copiar_no(indice->lista[k-1], indice->lista[k]);
            
            // removendo ultimo indice
            free(indice->lista[k-1]);
            
            // atualizando tamanho do indice
            indice->tamanho--;

            // atualizando o vetor de indices
            indice->lista = (NO**)realloc(indice->lista, sizeof(NO*) * indice->tamanho);
        
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

int inserirFF(FILE* file, INDICE* indice, Registro* reg) {
    long int referencia;

    // inserindo registro no arquivo de dados
    referencia = _inserirFF_dado(file, reg);

    // inserindo referencia no arquivo de indice
    _inserirFF_indice(indice, reg->cnpj, referencia);

    return(1);
}

long int _inserirFF_dado(FILE* file, Registro* reg){
    int sizeReg;
    int fragInt;
    long int atual;
    long int prox;
    long int ant;
    long int posInsert = -1;

    // calcula tamanho do registro
    sizeReg  = SIZE_CNPJ*2 + SIZE_DATA*2;
    sizeReg += strlen(reg->razSoc);
    sizeReg += strlen(reg->nomeFant);
    sizeReg += strlen(reg->motCanc);
    sizeReg += strlen(reg->nomeEmp);
    
    // Captura a posição (através de ant, atual e prox) na inserção interna no arquivo de dados
    // e retorna a fragmentação interna
    fragInt = _getFragAndPosFF(file, sizeReg, &ant, &atual, &prox);

    // insere no meio do arquivo, encontrou um espaço de remoção lógica
    if (fragInt != -1) {

        // fseek para o inicio do espaço de inserção
        fseek(file, atual, SEEK_SET);

        // salva a posição de inserção
        posInsert = atual;

        // grava o registro
        salvarRegistro(reg, file);

        // trata a fragmentação interna
        _tratarFragIntFF(file, fragInt, sizeReg, &atual, &prox);

        // encontra o Byte Ofsset para atualização do anterior 
        if (ant == -1)
            // fseek para cabeça da lista
            fseek(file, 0, SEEK_SET);                
        else 
            // fseek para o item anterior da lista
            fseek(file, ant + sizeof(char) + sizeof(int), SEEK_SET);
        
        // grava o próximo no anterior
        fwrite(&prox, sizeof(long int), 1, file);
    }
    // insere no fim do arquivo, não encontrou um espaço de remoção lógica
    else {

        // fseek para o fim do arquivo
        fseek(file, 0, SEEK_END);

        // salva a posição de inserção
        posInsert = ftell(file);

        // grava o registro
        salvarRegistro(reg, file);
    }

    return(posInsert);
}

int _getFragAndPosFF(FILE* file, int sizeReg, long int* antP, long int* atualP, long int *proxP){
    long int ant, atual, prox;
    int encontrou = 0;
    int sizeDisp;

    // anterior ao inicio da lista
    ant = -1;

    // fseek para o inicio do arquivo
    fseek (file, 0, SEEK_SET);

    // le a cabeça da lista de remoção (byte offset do registro topo da lista)
    fread(&atual, sizeof(long int), 1, file);

    while (atual != -1){

        // fseek para a area de inserção
        fseek (file, atual + sizeof(char), SEEK_SET);

        // le o espaço disponível
        fread(&sizeDisp, sizeof(int), 1, file);
        
        // exclui o espaço do delimitador
        sizeDisp -= sizeof(char);

        // le qual é o próximo registro da lista
        fread(&prox, sizeof(long int), 1, file);

        // se tem espaço suficiente para o registro
        if (sizeReg <= sizeDisp){
            encontrou = 1;
            break;
        }

        ant = atual;
        atual = prox;
    }

    // atualiza ponteiros
    *antP = ant;
    *atualP = atual;
    *proxP = prox;

    if (encontrou)
        // retorna a framgmentação interna
        return(sizeDisp - sizeReg);
    else
        return(-1);
}

void _tratarFragIntFF(FILE* file, int fragInt, int sizeReg, long int* atual, long int* prox){
    char exc_log = EXC_LOG;
    char del_reg = DEL_REG;

    // verifica se é possível adicionar a fragmentação interna a lista de excluidos
    if (fragInt >= (sizeof(char) + sizeof(int) + sizeof(long int))) {

        // grava o caracter de exclusão lógica
        fwrite(&exc_log, sizeof(char), 1, file);

        // grava o tamanho da fragmentação interna
        fwrite(&fragInt, sizeof(int), 1, file);

        // grava o byte ofssfrag - delimitadorfrag - delimitadoret do próximo registro removido
        fwrite(prox, sizeof(long int), 1, file);

        // atribuição para que o próximo do anterior seja a fragmentação interna
        *prox = *atual + sizeReg + sizeof(char);
    }
}

/*
    Descrição:

	Parâmetros:
*/
void _inserirFF_indice(INDICE* indice, char* chave, long int referencia){
    int i;
    int k;
    int cmp;
    int removeu = 0;

    for (i=0; i <= indice->tamanho; i++){
        
        if (i != indice->tamanho)
            cmp = strcmp(chave, indice->lista[i]->chave);

        if ((cmp < 0) || (i == indice->tamanho)) {

            // alocando novo indica
            indice->lista = (NO**)realloc(indice->lista, sizeof(NO*) * (indice->tamanho + 1));
            indice->lista[indice->tamanho] = (NO*) malloc(sizeof(NO));
            
            // copiando chave e referencia para o novo indice
            strcpy(indice->lista[k]->chave, chave);
            indice->lista[k]->referencia = referencia;

            // reordenando indice quando necessário
            if (i != indice->tamanho)
                indice = atualizar_indice(indice);

            // atualizando tamanho do indice
            indice->tamanho++;

            break;
        }
    }
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
    // copia dados do nó b para nó a
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
        printf("\t%ld\n", indice->lista[i]->referencia);
    }
}

int pesquisa_indice(INDICE* indice, char* chave){
    int i;
    int cmp;
    long int referencia = -1;

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