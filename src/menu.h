/*
GRUPO G5
Felipe Scrochio Custodio, 9442688
Felipe Contini Torres, 9253670
Júlia Diniz Ferreira, 9364865
Bruno Henrique Rasteiro, 9292910
*/

/********************************************//**
 *  Funções para as opções do menu principal
 ***********************************************/

#ifndef MENU_H
#define MENU_H

#include <stdio.h>
#include "funcoes.h"

void opcao1(FILE *file);
void opcao2(FILE *file);
int validaArquivos(FILE **fileIn, FILE **fileOut1, FILE **fileOut2, FILE **fileOut3);

#endif
