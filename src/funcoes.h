/*
GRUPO G5
Felipe Scrochio Custodio, 9442688
Felipe Contini Torres, 9253670
Júlia Diniz Ferreira, 9364865
Bruno Henrique Rasteiro, 9292910
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parametros.h"

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

//*******************//
//* FUNÇÕES DO MAIN *//
//*******************//
int validaArquivos(FILE **fileIn, FILE **fileOut1, FILE **fileOut2, FILE **fileOut3);

void opcao1(FILE *file);

void opcao2(FILE *file);

//-------------------//


//*****************************************************//
//* FUNÇÕES PARA GERAR O ARQUIVO BINÁRIO DE REGISTROS *//
//*****************************************************//
void checkSizeFixedFields(Registro *reg);

void csv2Bin(FILE *fileIn, FILE *file1, FILE *file2, FILE *file3);

void saveField(Registro *reg, FILE *file);

void saveReg(Registro *reg, FILE *file);
//-----------------------------------------------------//


//***************************************//
//* FUNÇÃO PARA LISTAGEM DOS REGISTROS *//
//***************************************//
void listBin(FILE *file);

//---------------------------------------//

//********************//
//* FUNÇÃO DE BUSCA *//
//********************//
Registro* buscaCampoCNPJ(FILE *file, char *strBusca);

//--------------------//

//****************************//
//* SUB FUNÇÕES - UTILIDADES *//
//****************************//
void addCharFieldTxt(Registro *reg, char c, int field, int iField);

void addCharFieldBin(Registro *reg, char c, int field, int iField);

void nullFields(Registro *reg);

void printReg(Registro *reg);

int compareFieldCNPJ(Registro *reg, char *strBusca);

//----------------------------//
