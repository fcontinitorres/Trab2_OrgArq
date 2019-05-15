/*
GRUPO G5
Felipe Scrochio Custodio, 9442688
Felipe Contini Torres, 9253670
Júlia Diniz Ferreira, 9364865
Bruno Henrique Rasteiro, 9292910
*/

#include "registro.h"

//************************************************//
//* FUNÇÕES PARA GERAR OS ARQUIVOS BINÁRIOS      *//
//************************************************//

/*	Descrição:
		Lê os dados em um arquivo de entrada e os salva de forma organizada em um arquivo de saída
	Parâmetros:
		entrada = Arquivo de entrada
		saida1 = Arquivo de saída
		saida2 = Arquivo de saída
		saida3 = Arquivo de saída
*/
void csv2Bin(FILE *entrada, FILE *saida1, FILE *saida2, FILE *saida3) {
    char c = 'c'; // caracter que irá percorrer o arquivo, deve ser inicializado com um valor != de EOF
    int field;  // indica qual campo do arquivo está sendo lido, seus valores vão de 0 a 7
    int iField; // indica o inidice o campo que está sendo lido, seus valores vão de 0 a n
    Registro reg; // armazena um registro lido

    // atribui null aos campos variáveis para fazer realloc deles
    anularCampos(&reg);

    // inicializando variáveis
    field = 0;
    iField = 0;

    // iterando sobre o arquivo lendo caracter por caracter até
    // alcançar fim de arquivo
    while(c != EOF) {

        // le um caracter do arquivo
        c = fgetc(entrada);

        // caracter de nova linha, deve ser ignorado na leitura
        if (c == '\r')
        	continue;

        // chegou em um novo registro
        if (c == '\n') {

        	// verifica se todos os campos fixos tem o tamanho correto
        	// certifica que entradas null fiquem com o tamanho do campo fixo
        	checarTamanhoCampoFixo(&reg);

        	// grava o registro anterior
	        salvarRegistro(&reg, saida1);
	        salvarRegistro(&reg, saida2);
	        salvarRegistro(&reg, saida3);

	        // zera indicador do campo que será lido
            field = 0;
            // zera indice do campo que será lido
            iField = 0;
            // prepara os campo para serem realocados
			free(reg.razSoc);
			free(reg.nomeFant);
			free(reg.motCanc);
			free(reg.nomeEmp);
            anularCampos(&reg);
            continue;
        }

        // chegou em um novo campo
        if (c == ';') {
            // incrementa qual campo o arquivo está lendo
            field++;
            // zera o indice do campo que será lido
            iField = 0;
            continue;
        }

        // adiciona o carácter lido no registro, no campo atual
        addCharFieldTxt(&reg, c, field, iField++);
    }
}

/*	Descrição:
		Função que verifica se os campos fixos tem o tamanho correto. Se não estiverem, são completados com 0.

	Parâmetros:
		reg = Registro contendo os campos a serem verificados
*/
void checarTamanhoCampoFixo(Registro *reg) {
	int size,i;

	// verifica o tamanho do campo cnpj
	if (strlen(reg->cnpj) < SIZE_CNPJ) {
		size = strlen(reg->cnpj);
		for(i=size; i<SIZE_CNPJ; i++) {
			reg->cnpj[i] = '0'; // completa o campo com 0
		}
		reg->cnpj[i] = '\0';
	}

	// verifica o tamanho do campo data de registro
	if( strlen(reg->dtReg) < SIZE_DATA) {
		size = strlen(reg->dtReg);
		for(i=size; i<SIZE_DATA ;i++) {
			reg->dtReg[i] = '0'; // completa o campo com 0
		}
		reg->dtReg[i] = '\0';
	}

	// verifica o tamanho do campo data de cancelamento
	if( strlen(reg->dtCanc) < SIZE_DATA) {
		size = strlen(reg->dtCanc);
		for(i=size; i<SIZE_DATA ;i++) {
			reg->dtCanc[i] = '0'; // completa o campo com 0
		}
		reg->dtCanc[i] = '\0';
	}

	// verifica o tamanho do campo cnpj auditor
	if( strlen(reg->cnpjAud) < SIZE_CNPJ) {
		size = strlen(reg->cnpjAud);
		for(i=size; i<SIZE_CNPJ ;i++) {
			reg->cnpjAud[i] = '0'; // completa o campo com 0
		}
		reg->cnpjAud[i] = '\0';
	}
}

/*	Descrição:
		Função que grava um registro no arquivo
	Parâmetros:
		reg = Registro que será gravado
		file = Arquivo a ser gravado
*/
void salvarRegistro(Registro *reg, FILE *file) {
	char delReg = DEL_REG; // delimitador de registro

	// grava todos os campos
	salvarCampo(reg, file);
	// grava o delimitador de registros
	fwrite(&delReg, sizeof(char), 1, file);
}

/*	Descrição:
		Dado um registro a função grava todos os seus campos no arquivo, seguindo o modelo de organização híbrida.
	Parâmetros:
		reg = Registro que será gravado
		file = Arquivo onde o registro será gravado
*/
void salvarCampo(Registro *reg, FILE *file) {
	int sizeField; // tamanho da string que será salva
	char delField = DEL_FIELD; // delimitador de campo

	/********************************************//**
     *  Campos fixos
     ***********************************************/

	// CNPJ
	sizeField = strlen(reg->cnpj); // calcula tamanho da string
	fwrite(reg->cnpj, sizeof(char), sizeField, file); // frwite string
	//if(org != 'I') fwrite(&delField, sizeof(char), 1, file);

	// Data do Registro
	sizeField = strlen(reg->dtReg);
	fwrite(reg->dtReg, sizeof(char), sizeField, file);
	//if(org != 'I') fwrite(&delField, sizeof(char), 1, file);

	// Data do Cancelamento
	sizeField = strlen(reg->dtCanc);
	fwrite(reg->dtCanc, sizeof(char), sizeField, file);
	//if(org != 'I') fwrite(&delField, sizeof(char), 1, file);

	// CNPJ da Empresa de Auditoria
	sizeField = strlen(reg->cnpjAud);
	fwrite(reg->cnpjAud, sizeof(char), sizeField, file);
	//if(org != 'I') fwrite(&delField, sizeof(char), 1, file);

	/********************************************//**
     *  Campos variáveis
     ***********************************************/

	// Nome Social
	sizeField = strlen(reg->razSoc);
	fwrite(reg->razSoc, sizeof(char), sizeField, file);
	fwrite(&delField, sizeof(char), 1, file);

	// Nome Fantasia
	sizeField = strlen(reg->nomeFant);
	fwrite(reg->nomeFant, sizeof(char), sizeField, file);
	fwrite(&delField, sizeof(char), 1, file);

	// Motivo do Cancelamento
	sizeField = strlen(reg->motCanc);
	fwrite(reg->motCanc, sizeof(char), sizeField, file);
	fwrite(&delField, sizeof(char), 1, file);

	// Nome da Empresa de Auditoria
	sizeField = strlen(reg->nomeEmp);
	fwrite(reg->nomeEmp, sizeof(char), sizeField, file);
	fwrite(&delField, sizeof(char), 1, file);
}
//-----------------------------------------------------//

//***************************************//
//*  FUNÇÕES PARA LISTAGEM DOS ARQUIVOS *//
//***************************************//

/*	Descrição:
		Lista todos os registros de um arquivo binário
	Parâmetros:
		file = Arquivo a ser lido
*/
void listarBinario(FILE *file) {
	Registro reg; // registro que será lido
	int field; // campo que está sendo lido (0 a 7)
	int iField;	// indice do campo que está sendo lido
	char c;	// carácter que irá percorrer todo o arquivo

	// inicializando variáveis
	field = 0;
	iField = 0;
	anularCampos(&reg);

    // leitura do arquivo até chegar em EOF
	do {
		// lê campos de tamanho fixo do
		// arquivo para o registro Reg
		fread(&reg.cnpj, sizeof(char), SIZE_CNPJ, file);
		fread(&reg.dtReg, sizeof(char), SIZE_DATA, file);
		fread(&reg.dtCanc, sizeof(char), SIZE_DATA, file);
		fread(&reg.cnpjAud, sizeof(char), SIZE_CNPJ, file);

		// lê campos de tamanho variavel
		while(fread(&c,sizeof(char),1,file) == 1) {
			// delimitador de registro encontrado
			// resetar estrutura Reg para receber novo registro
			if (c == DEL_REG) {
				field = 0;
				iField = 0;
				printRegistro(&reg);
				free(reg.razSoc);
				free(reg.nomeFant);
				free(reg.motCanc);
				free(reg.nomeEmp);

				anularCampos(&reg);
				break;
			}

			// campo novo
			if (c == DEL_FIELD) {
				field++; // avança para o próximo campo do registro
				iField = 0;
				continue;
			}

			// adiciona char ao campo
			addCharFieldBin(&reg, c, field, iField++);
            // iField é o índice da string (vetor de char)
            // onde o novo caracter será inserido
		}

	} while (!feof(file));
}

//********************//
//* FUNÇÕES DE BUSCA *//
//********************//

/*	Descrição:
		Função que compara o campo CNPJ de um registro com um valor informado
	Parâmetros:
		reg = Registro cujo campo CNPJ irá sofrer a comparação
		strBusca = Valor (string) que será comparado com o campo do registro dado
	Retorno:
		Retorna o resultado da comparação, 0 para valores iguais e 1 caso contrário
*/
int compararCNPJ(Registro *reg, char *strBusca) {
	int i;

	if (reg->cnpj != NULL)
		for(i=0; i<SIZE_CNPJ; i++) {
			if(reg->cnpj[i] != strBusca[i]) {
				return 1;
			}
		}
	return 0;
}

/*	Descrição:
		Faz uma busca no arquivo de acordo com o campo e valor informado,
		necessáriamente os registros do arquivo devem ser organizados por um delimitador entre eles
	Parâmetros:
		file = Arquivo que a busca será realizada
		fieldBusca = Campo que a busca irá comparar com o valor informado
		strBusca = Valor (string) de comparação da busca
	Retorno:
		Retorna o registro caso ele seja encontrado, caso contrário retorna NULL
*/
Registro* buscaCampoCNPJ(FILE *file, char *strBusca) {
	char c; // carácter que irá percorrer todo o arquivo
	Registro *reg; // registro que será retornado
	int field; // campo variaveis que serao lidos (4 a 7)
	int iField; // indice do campo que está sendo lido

	// aloca e prepara um registro
	reg = (Registro *) malloc(sizeof(Registro));
	// inicializando variáveis
	field = 4; // CNPJ
	iField = 0;
	anularCampos(reg);

	do {
		fread(&reg->cnpj,sizeof(char),SIZE_CNPJ,file);
		fread(&reg->dtReg,sizeof(char),SIZE_DATA,file);
		fread(&reg->dtCanc,sizeof(char),SIZE_DATA,file);
		fread(&reg->cnpjAud,sizeof(char),SIZE_CNPJ,file);

		while(fread(&c,sizeof(char),1,file) == 1) {
			// registro novo
			if (c == DEL_REG) {

                // após ter lido o registro e chegado ao
                // delimitador, compara o CNPJ do registro
                // lido com a string de busca
				if (compararCNPJ(reg, strBusca) == 0)
					return reg;

				field = 4;
				iField = 0;
				free(reg->razSoc);
				free(reg->nomeFant);
				free(reg->motCanc);
				free(reg->nomeEmp);

				anularCampos(reg);
				break;
			}

			// campo novo
			if (c == DEL_FIELD) {
				field++;
				iField = 0;
				continue;
			}

			// adiciona char ao campo
			addCharFieldBin(reg, c, field, iField++);
		}

	} while (!feof(file));

	// não encontrou
	free(reg->razSoc);
	free(reg->nomeFant);
	free(reg->motCanc);
	free(reg->nomeEmp);
	free(reg);
	return NULL;
}

/*	Descrição:
		Dado um registro esta função atribui NULL aos seus campos de tamanho variável.
		É utilizada quando um registro é recém criado e seus campos variáveis irão passar por um realloc
	Parâmetros:
		reg = Registro que irá receber null em seus campos variáveis
*/
void anularCampos(Registro *reg) {
    reg->razSoc  = NULL;
    reg->nomeFant = NULL;
    reg->motCanc  = NULL;
    reg->nomeEmp  = NULL;
}

/*	Descrição:
		Função utilizada para preencher os campos variáveis lendo o arquivo de entrada,
		sua função é inserir um char em um campo,
		dado o registro, o campo e sua posição no campo
	Parâmetros:
		reg = Registro que terá o campo com o char adicionado
		c = Char que será adicionado
		field = Indica qual campo terá o char adicionado (seus valores vão de 0 a 7)
		iField = Indice que indica em qual posição do campo o char será inserido
*/
void addCharFieldTxt(Registro *reg, char c, int field, int iField) {
    switch(field) {
        // CNPJ
        case 0:
            reg->cnpj[iField] = c;
            reg->cnpj[++iField] = '\0';
        break;

        // Nome Social
        case 1:
            reg->razSoc = (char*)realloc(reg->razSoc, sizeof(char) * iField+2);
            reg->razSoc[iField] = c;
            reg->razSoc[++iField] = '\0';
        break;

        // Nome Fantasia
        case 2:
            reg->nomeFant = (char*)realloc(reg->nomeFant, sizeof(char) * iField+2);
            reg->nomeFant[iField] = c;
            reg->nomeFant[++iField] = '\0';
        break;

        // Data do Registro
        case 3:
            reg->dtReg[iField++] = c;
            reg->dtReg[iField] = '\0';
        break;

        // Data do Cancelamento
        case 4:
            reg->dtCanc[iField++] = c;
            reg->dtCanc[iField] = '\0';
        break;

        // Motivo do Cancelamento
        case 5:
            reg->motCanc = (char*)realloc(reg->motCanc, sizeof(char) * iField+2);
            reg->motCanc[iField] = c;
            reg->motCanc[++iField] = '\0';
        break;

        // Nome da Empresa de Auditoria
        case 6:
            reg->nomeEmp = (char*)realloc(reg->nomeEmp, sizeof(char) * iField+2);
            reg->nomeEmp[iField] = c;
            reg->nomeEmp[++iField] = '\0';
        break;

        // CNPJ da Empresa de Auditoria
        case 7:
            reg->cnpjAud[iField++] = c;
            reg->cnpjAud[iField] = '\0';
        break;
    }
}

/*	Descrição:
		Função utilizada para preencher os campos variáveis,
		sua função é inserir um char em um campo dos possiveis variaveis,
		dado o registro, o campo e sua posição no campo
	Parâmetros:
		reg = Registro que terá o campo com o char adicionado
		c = Char que será adicionado
		field = Indica qual campo terá o char adicionado (seus valores vão de 0 a 7)
		iField = Indice que indica em qual posição do campo o char será inserido */
void addCharFieldBin(Registro *reg, char c, int field, int iField) {
    switch(field) {

        // Nome Social
        case 0:
            reg->razSoc = (char*)realloc(reg->razSoc, sizeof(char) * iField+2);
            reg->razSoc[iField] = c;
            reg->razSoc[++iField] = '\0';
        break;

        // Nome Fantasia
        case 1:
            reg->nomeFant = (char*)realloc(reg->nomeFant, sizeof(char) * iField+2);
            reg->nomeFant[iField] = c;
            reg->nomeFant[++iField] = '\0';
        break;

        // Motivo do Cancelamento
        case 2:
            reg->motCanc = (char*)realloc(reg->motCanc, sizeof(char) * iField+2);
            reg->motCanc[iField] = c;
            reg->motCanc[++iField] = '\0';
        break;

        // Nome da Empresa de Auditoria
        case 3:
            reg->nomeEmp = (char*)realloc(reg->nomeEmp, sizeof(char) * iField+2);
            reg->nomeEmp[iField] = c;
            reg->nomeEmp[++iField] = '\0';
        break;
    }
}

/*	Descrição:
		Exibe um registro na saída padrão.
	Parâmetros:
		reg = Registro que será exibido na saída padrão */
void printRegistro(Registro *reg) {

	int i;

	printf("CNPJ.........: ");
	if(reg->cnpj[0] == 'n') {
		printf("null\n");
	}
	else{
		for(i=0; i<SIZE_CNPJ; i++) {
			printf("%c",reg->cnpj[i]);
		}
		printf("\n");
	}

	printf("Razão Social.: %s\n",   reg->razSoc);
	printf("Nome Fantasia: %s\n",   reg->nomeFant);

	printf("Data Registro: ");
	if(reg->dtReg[0] == 'n') {
		printf("null\n");
	}
	else {
		for(i=0; i<SIZE_DATA; i++) {
			printf("%c",reg->dtReg[i]);
		}
		printf("\n");
	}

	printf("Data Canc....: ");
	if(reg->dtCanc[0] == 'n') {
		printf("null\n");
	}
	else{
		for(i=0; i<SIZE_DATA; i++) {
			printf("%c", reg->dtCanc[i]);
		}
		printf("\n");
	}

	printf("Motivo Canc..: %s\n",   reg->motCanc);
	printf("Empresa Aud..: %s\n",   reg->nomeEmp);

	printf("CNPJ Emp Aud.: ");
	if(reg->cnpjAud[0] == 'n') {
		printf("null\n\n");
	}
	else{
		for(i=0;i<SIZE_CNPJ;i++) {
			printf("%c", reg->cnpjAud[i]);
		}
		printf("\n\n");
	}
}


void printArq(FILE* file){
	char c;
	printf("\n\n+++++++++ARQUIVO++++++++\n");

	do{
		c = fgetc(file);
		printf("%c", c);
		if(c == '#') printf("\n");
	}while(!feof(file));
	printf("\n\n\n");
}
