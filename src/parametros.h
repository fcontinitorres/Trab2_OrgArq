/*
GRUPO G5
Felipe Scrochio Custodio, 9442688
Felipe Contini Torres, 9253670
Júlia Diniz Ferreira, 9364865
Bruno Henrique Rasteiro, 9292910
*/

/********************************************//**
 *  Variáveis globais
 *  Caminhos de arquivos
 *  Definições de tamanhos e delimitadores
 ***********************************************/

// tamanho dos campos fixos
#define SIZE_CNPJ 18
#define SIZE_DATA 8

// delimitador do registro no arquivo de saí­da
#define DEL_REG '#'

// delimitador do campo arquivo de saída
#define DEL_FIELD ';'

// caracter para marcar exclusão lógica de registro no arquivo de dados
#define EXC_LOG '@'

// caminho do arquivo de entrada
#define FILE_IN "data/dados.csv"

// caminho do arquivo de saida
#define FILE_OUT1 "out/saida1.bin"
#define FILE_OUT2 "out/saida2.bin"
#define FILE_OUT3 "out/saida3.bin"
#define FILE_IND1 "out/indice1.bin"
#define FILE_IND2 "out/indice2.bin"
#define FILE_IND3 "out/indice3.bin"
