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
#include "registro.h"

void opcao1(FILE *file);
void opcao2(FILE *file);
int validaArquivos(FILE **entrada, FILE **saida1, FILE **saida2, FILE **saida3);

#endif
