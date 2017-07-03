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
#include <ctype.h>

#include "registro.h"
#include "parametros.h"

/********************************************//**
 *  Definição de nó para lista (índice)
 *  Funções para manipulação de lista de nós
 ***********************************************/

typedef struct no {

    char chave[SIZE_CNPJ + 1]; // chave de busca (CNPJ)
    long int referencia; // byte offset

} NO;

typedef struct indice {

    NO** lista; // lista de nós
    int tamanho; // mantém tamanho atualizado

} INDICE;

//***************************************************//
//*  Funções para criação e manipulação de índices  *//
//***************************************************//
int abre_indices(FILE** indice1, FILE** indice2, FILE** indice3);
INDICE* ler_indice(FILE* arquivo, INDICE* indice); // ler arquivo de índice
void escrever_indice(INDICE* indice, FILE* arquivo); // escrever arquivo de índice
INDICE* criar_indices(FILE* saida); // gerar arquivos de índice iniciais
void _inserir_indice(INDICE* indice, char* chave, long int referencia); // inserir novo item no índice
INDICE* atualizar_indice(INDICE* indice); // reordenar
void imprimir_indice(INDICE* indice); // imprime estrutura de índice completa
long int _pesquisa_indice(INDICE* indice, char* chave);
void destruir_indice(INDICE**); // desalocar

//*************************//
//* FUNÇÕES PARA REMOÇÃO  *//
//*************************//
int remover(FILE* file, INDICE* indice, char* chave);
void _remover_dado(FILE* file, long int referencia);
int _remover_indice(INDICE* indice, char* chave);

//**************************//
//* FUNÇÕES PARA INSERÇÃO  *//
//**************************//
int inserirFirstFit(FILE* file, INDICE* indice, Registro* reg);
long int _inserirFirstFit_dado(FILE* file, Registro* reg);
int _getFragAndPosFF(FILE* file, int sizeReg, long int* antP, long int* atualP, long int *proxP);
void _tratarFragIntFF(FILE* file, int fragInt, int sizeReg, long int* atual, long int* prox);
void _inserirFirstFit_indice(INDICE* indice, char* chave, long int referencia);

//************************//
//* FUNÇÕES UTILITÁRIAS  *//
//************************//
NO* copiar_no(NO* a, NO* b); // auxiliar para ordenação
long int converter_CNPJ(char* CNPJ);

#endif
