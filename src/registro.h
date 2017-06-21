/*
GRUPO G5
Felipe Scrochio Custodio, 9442688
Felipe Contini Torres, 9253670
Júlia Diniz Ferreira, 9364865
Bruno Henrique Rasteiro, 9292910
*/

/********************************************//**
 *  Definição de registro
 *  Funções para manipulação do registro
 ***********************************************/

#ifndef REGISTRO_H
#define REGISTRO_H

//*************************************************//
//* ESTRUTURA DE DADOS QUE REPRESENTA UM REGISTRO *//
//*************************************************//
typedef struct {
    char cnpj[19];
    char dtReg[9];
    char dtCanc[9];
	char cnpjAud[19];
    char *razSoc;
    char *nomeFant;
    char *motCanc;
    char *nomeEmp;
} Registro;

#endif
