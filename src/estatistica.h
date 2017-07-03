/*
GRUPO G5
Felipe Scrochio Custodio, 9442688
Felipe Contini Torres, 9253670
Júlia Diniz Ferreira, 9364865
Bruno Henrique Rasteiro, 9292910
*/

#ifndef ESTATISTICA_H
#define ESTATISTICA_H

#include <stdio.h>
#include <string.h>

#include "indice.h"

//*****************************************************//
//* FUNÇÕES PARA REALIZAR AS OPÇÕES 4 E 5 DO MENU     *//
//*****************************************************//

void listarRemovidos(FILE *file);
void indices_estat(INDICE* indiceR1, INDICE* indiceR2, INDICE* indiceR3);

#endif //ESTATISTICA_H
