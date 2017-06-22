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

#include "registro.h"

typedef struct node NODE;

struct node {

    char chave[19]; // chave de busca (CNPJ)
    char referencia[4]; // chave de referencia (byte offset)

};

void pesquisa_indice_chave(FILE*, char*); // busca por chave
void pesquisa_indice_ref(FILE*, char*); // busca por referência
void criar_indices(FILE*, FILE*, FILE*, FILE*); // alocar estruturas e gerar arquivos de índice
void inserir_indice(); // inserir novo item no índice
void remover_indice(); // remover item do índice
void atualizar_indice(FILE*); // reordenar
void destruir_indice(FILE*); // desalocar

#endif
