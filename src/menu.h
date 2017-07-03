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
#include "indice.h"


void opcao1(FILE *file);
void opcao2(INDICE* indice1, INDICE* indice2, INDICE* indice3, FILE* saida1, FILE* saida2, FILE* saida3);
void opcao3(INDICE* indice1, INDICE* indice2, INDICE* indice3, FILE* saida1, FILE* saida2, FILE* saida3);
void opcao4(INDICE* r1, INDICE* r2, INDICE* r3);
void opcao5();
int abre_saidas(FILE **entrada, FILE **saida1, FILE **saida2, FILE **saida3);

#endif
