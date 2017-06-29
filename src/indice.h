/*
GRUPO G5
Felipe Scrochio Custodio, 9442688
Felipe Contini Torres, 9253670
Júlia Diniz Ferreira, 9364865
Bruno Henrique Rasteiro, 9292910
*/

/********************************************//**
 *  Funções para a criação e manipulação dos
 *  índices primários
 ***********************************************/

#ifndef INDICE_H
#define INDICE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "registro.h"

/********************************************//**
 *  Definição de nó para lista (índice)
 *  Funções para manipulação de lista de nós
 ***********************************************/

typedef struct no {

    char chave[SIZE_CNPJ + 1]; // chave de busca (CNPJ)
    int referencia; // byte offset

} NO;

typedef struct indice {

    NO** lista;
    int tamanho;

} INDICE;

/********************************************//**
 *  Funções para criação e manipulação de índices
 ***********************************************/

//int pesquisa_indice_chave(FILE*, char*); // busca por chave
//void pesquisa_indice_ref(FILE*, int); // busca por referência
int pesquisa_indice(INDICE* indice, char* chave);
int criar_indices(FILE* saida, FILE* ind1, FILE* ind2, FILE* ind3); // gerar arquivos de índice iniciais
void inserir_indice(); // inserir novo item no índice
int remover(FILE* file, INDICE* indice, char* chave); // remover um registro (remove tanto no indice quanto no arquivo de dados)
int remover_indice(INDICE* indice, char* chave); // remover item do índice
void remover_dado(FILE* file, int referencia); // remove logicamente o registro do arquivo de dados
NO* copiar_no(NO* a, NO* b); // auxiliar para ordenação
INDICE* atualizar_indice(INDICE* indice); // reordenar
void destruir_indice(FILE*); // desalocar
void imprimir_indice(INDICE* indice); // imprime o indice no terminal

#endif
