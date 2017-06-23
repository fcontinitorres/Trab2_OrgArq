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

void pesquisa_indice_chave(FILE*, char*); // busca por chave
void pesquisa_indice_ref(FILE*, char*); // busca por referência
int criar_indices(FILE* saida, FILE* ind1, FILE* ind2, FILE* ind3); // alocar estruturas e gerar arquivos de índice
void inserir_indice(); // inserir novo item no índice
void remover_indice(); // remover item do índice
void atualizar_indice(FILE*); // reordenar
void destruir_indice(FILE*); // desalocar
char* int2string(int val, int base); // transforma int em string

#endif
