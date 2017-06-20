/*
GRUPO G5
Felipe Scrochio Custodio, 9442688
Felipe Contini Torres, 9253670
Júlia Diniz Ferreira, 9364865
Bruno Henrique Rasteiro, 9292910
*/

#include "../src/funcoes.h"

//*******************//
//* FUNÇÕES DO MAIN *//
//*******************//

/*	Descrição:
		Abre os arquivos de entrada e saída do programa
	Parêmetros:
		fileIn = arquivo de entrada
		fileOut1 = arquivo de saída
		fileOut2 = arquivo de saída
		fileOut3 = arquivo de saída
	Retorno:
		Retorna um int que indica o sucesso da função. Os possíves valores são:
		1 -> Arquivos aberto com sucesso
		2 -> Falha ao abrir arquivos */
int validaArquivos(FILE **fileIn, FILE **fileOut1, FILE **fileOut2, FILE **fileOut3) {

	// abrindo arquivos
    *fileIn  = fopen(FILE_IN, "r");
    *fileOut1 = fopen(FILE_OUT1, "wb+");
    *fileOut2 = fopen(FILE_OUT2, "wb+");
    *fileOut3 = fopen(FILE_OUT3, "wb+");

    if (*fileIn == NULL) {
    	printf("Erro ao abrir: %s\n", FILE_IN);
    	return(0);
    }

    if (*fileOut1 == NULL) {
    	printf("Erro ao abrir: %s\n", FILE_OUT1);
    	return(0);
    }

    if (*fileOut2 == NULL) {
    	printf("Erro ao abrir: %s\n", FILE_OUT2);
    	return(0);
    }

    if (*fileOut3 == NULL) {
    	printf("Erro ao abrir: %s\n", FILE_OUT3);
    	return(0);
    }

    return(1);
}

/*	Descrição:
		Realiza as tarefas para executar a primeira opção do menu
	Parêmetros:
		file = arquivo de saida (binário) */
void opcao1(FILE *file) {
	// lista todos os dados
	listBin(file);

	// reseta ponteiro do arquivo
	fseek(file, 0, SEEK_SET);
}

/*	Descrição:
		Realiza as tarefas para executar a segunda opção do menu
	Parêmetros:
		file = arquivo de saida (binário) */
void opcao2(FILE *file) {
	char *strBusca; // valor que a busca irá usar para comparar
	Registro *reg; // registro resultante da busca

	// espera o valor da busca
	printf("Informe o cnpj a ser buscado: ");
	strBusca = NULL;
	scanf("%ms", &strBusca);

	// busca
	reg = buscaCampoCNPJ(file, strBusca);

	if (reg != NULL) {
		printf("Registro Localizado\n");
		printReg(reg);
		free(reg->razSoc);
		free(reg->nomeFant);
		free(reg->motCanc);
		free(reg->nomeEmp);
	}
	else
		printf("Não foi possível localizar o registro.\n");

	// reseta ponteiro do arquivo
	fseek(file, 0, SEEK_SET);
	free(reg);
	free(strBusca);
}


//************************************************//
//* FUNÇÕES PARA GERAR OS ARQUIVOS BINÁRIOS      *//
//************************************************//

/*	Descrição:
		Lê os dados em um arquivo de entrada e os salva de forma organizada em um arquivo de saída
	Parêmetros:
		fileIn = Arquivo de entrada
		fileOut1 = Arquvio de saída
		fileOut2 = Arquvio de saída
		fileOut3 = Arquvio de saída */
void csv2Bin(FILE *fileIn, FILE *fileOut1, FILE *fileOut2, FILE *fileOut3) {
    char c = 'c'; // caracter que irá percorrer o arquivo, deve ser inicializado com um valor != de EOF
    int field;  // indica qual campo do arquivo está sendo lido, seus valores vão de 0 a 7
    int iField; // indica o inidice o campo que está sendo lido, seus valores vão de 0 a n
    Registro reg; // armazena um registro lido

    // incializando variáveis
    // atribui null aos campos variáveis para fazer realloc deles
    nullFields(&reg);
    field = 0;
    iField = 0;

    // iterando sobre o arquivo
    while(c != EOF) {

        // le um caracter do arquivo
        c = fgetc(fileIn);

        // caracter de nova linha, deve ser ignorado na leitura
        if (c == '\r')
        	continue;

        // chegou em um novo registro
        if (c == '\n') {

        	// verifica se todos os campos fixos tem o tamanho correto
        	// certifica que entradas null tambem possui o tamanho fixo do campo
        	checkSizeFixedFields(&reg);

        	// grava o registro anterior
	        saveReg(&reg, fileOut1);
	        saveReg(&reg, fileOut2);
	        saveReg(&reg, fileOut3);

	        // zera indicador do campo que será lido
            field = 0;
            // zera indice do campo que será lido
            iField = 0;
            //Prepara os campo para serem realocados
			free(reg.razSoc);
			free(reg.nomeFant);
			free(reg.motCanc);
			free(reg.nomeEmp);
            nullFields(&reg);
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

        // adiciona o carácter lido no registro
        addCharFieldTxt(&reg, c, field, iField++);
    }
}

/*	Descrição:
		Função que verifica se os campos fixos tem o tamanho correto
	Parêmetros:
		reg = Registro a verificar o tamanho dos campos fixos*/
void checkSizeFixedFields(Registro *reg) {
	int size,i;

	//verifica o tamanho do campo cnpj
	if( strlen(reg->cnpj) < SIZE_CNPJ ) {
		size = strlen(reg->cnpj);
		for(i=size; i<SIZE_CNPJ; i++) {
			reg->cnpj[i] = '0'; //completa o campo com 0s
		}
		reg->cnpj[i] = '\0';
	}

	//verifica o tamanho do campo data de registro
	if( strlen(reg->dtReg) < SIZE_DATA) {
		size = strlen(reg->dtReg);
		for(i=size; i<SIZE_DATA ;i++) {
			reg->dtReg[i] = '0'; //completa o campo com 0s
		}
		reg->dtReg[i] = '\0';
	}

	//verifica o tamanho do campo data de cancelamento
	if( strlen(reg->dtCanc) < SIZE_DATA) {
		size = strlen(reg->dtCanc);
		for(i=size; i<SIZE_DATA ;i++) {
			reg->dtCanc[i] = '0'; //completa o campo com 0s
		}
		reg->dtCanc[i] = '\0';
	}

	//verifica o tamanho do campo cnpj auditor
	if( strlen(reg->cnpjAud) < SIZE_CNPJ) {
		size = strlen(reg->cnpjAud);
		for(i=size; i<SIZE_CNPJ ;i++) {
			reg->cnpjAud[i] = '0'; //completa o campo com 0s
		}
		reg->cnpjAud[i] = '\0';
	}
}

/*	Descrição:
		Função que grava um registro no arquivo
	Parêmetros:
		reg = Registro que será gravado
		file = Arquvio cujo registro será gravado */
void saveReg(Registro *reg, FILE *file) {
	int sizeReg; // tamanho do registro que será gravado
	char delReg = DEL_REG; // delimitador de registro

	// grava todos os campos
	saveField(reg, file);
	// grava o delimitador de registros
	fwrite(&delReg, sizeof(char), 1, file);
}

/*	Descrição:
		Dado um registro a função grava todos os seus campos no arquivo
	Parêmetros:
		reg = Registro que terá seus campos gravados
		file = Arquvio cujos campos serão gravados */
void saveField(Registro *reg, FILE *file) {
	int sizeField; // tamanho da string que será salva
	char delField = DEL_FIELD; // delimitador de campo

	// Campos Fixos
	// CNPJ
	sizeField = strlen(reg->cnpj);				  	  // calcula tamanho da string
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


	// Campos Variaveis
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
//* FUNÇÕES PARA LISTAGEM DOS REGISTROS *//
//***************************************//

/*	Descrição:
		Lista todos os registros de um arquivo estejam organizados por delimitadores
	Parêmetros:
		file = Arquivo que terá seus registros listados */
void listBin(FILE *file) {
	Registro reg; // registro que será listado
	int field; // campo que está sendo lido (0 a 7)
	int iField;	// indice do campo que está sendo lido
	char c;	// carácter que irá percorrer todo o arquivo


	// inicializando variáveis
	field = 0;
	iField = 0;
	nullFields(&reg);

	do{
		// Le campo de tamanho fixo
		fread(&reg.cnpj, sizeof(char), SIZE_CNPJ, file);
		fread(&reg.dtReg, sizeof(char), SIZE_DATA, file);
		fread(&reg.dtCanc, sizeof(char), SIZE_DATA, file);
		fread(&reg.cnpjAud, sizeof(char), SIZE_CNPJ, file);

		// Le campos de tamanho variavel
		while(fread(&c,sizeof(char),1,file) == 1){
			// registro novo
			if (c == DEL_REG) {	
				field = 0;
				iField = 0;
				printReg(&reg);
				free(reg.razSoc);
				free(reg.nomeFant);
				free(reg.motCanc);
				free(reg.nomeEmp);

				nullFields(&reg);
				break;
			}

			// campo novo
			if (c == DEL_FIELD) {
				field++;
				iField = 0;
				continue;
			}

			// adiciona char ao campo
			addCharFieldBin(&reg, c, field, iField++);
		}

	}while (!feof(file));
}

//********************//
//* FUNÇÕES DE BUSCA *//
//********************//

/*	Descrição:
		Faz uma busca no arquivo, independente da sua organização
		e de acordo com o campo e o valor informado
	Parêmetros:
		file = Arquivo que a busca será realizada
		fieldBusca = Campo que a busca irá comparar com o valor informado
		strBusca = Valor (string) de comparação da busca
	Retorno:
		Retorna o registro caso ele seja encontrado, caso contrário retorna NULL */


/*	Descrição:
		Faz uma busca no arquivo de acordo com o campo e valor informado,
		necessáriamente os registros do arquivo devem ser organizados por um delimitador entre eles
	Parêmetros:
		file = Arquivo que a busca será realizada
		fieldBusca = Campo que a busca irá comparar com o valor informado
		strBusca = Valor (string) de comparação da busca
	Retorno:
		Retorna o registro caso ele seja encontrado, caso contrário retorna NULL */
Registro* buscaCampoCNPJ(FILE *file, char *strBusca) {
	char c; // carácter que irá percorrer todo o arquivo
	Registro *reg; // registro que será retornado
	int field; // campo variaveis que serao lidos (4 a 7)
	int iField; // indice do campo que está sendo lido

	// aloca e prepara o registro
	reg = (Registro *) malloc(sizeof(Registro));
	// inicializando variáveis
	field = 4;
	iField = 0;
	nullFields(reg);

	do{
		fread(&reg->cnpj,sizeof(char),SIZE_CNPJ,file);
		fread(&reg->dtReg,sizeof(char),SIZE_DATA,file);
		fread(&reg->dtCanc,sizeof(char),SIZE_DATA,file);
		fread(&reg->cnpjAud,sizeof(char),SIZE_CNPJ,file);

		while(fread(&c,sizeof(char),1,file) == 1){
			// registro novo
			if (c == DEL_REG) {
				
				if (compareFieldCNPJ(reg, strBusca) == 0)
					return reg;

				field = 4;
				iField = 0;
				free(reg->razSoc);
				free(reg->nomeFant);
				free(reg->motCanc);
				free(reg->nomeEmp);

				nullFields(reg);
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

	}while (!feof(file));

	// não encontrou
	free(reg->razSoc);
	free(reg->nomeFant);
	free(reg->motCanc);
	free(reg->nomeEmp);
	free(reg);
	return NULL;
}

//*******************************//
//* FUNÇÕES ÚTEIS (SUB FUNÇÕES) *//
//*******************************//

/*	Descrição:
		Função utilizada para preencher os campos variáveis lendo o arquivo de entrada,
		sua função é inserir um char em um campo,
		dado o registro, o campo e sua posição no campo
	Parêmetros:
		reg = Registro que terá o campo com o char adicionado
		c = Char que será adicionado
		field = Indica qual campo terá o char adicionado (seus valores vão de 0 a 7)
		iField = Indice que indica em qual posição do campo o char será inserido */
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

        // Data do Registro
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
	Parêmetros:
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
		Dado um registro esta função atribui NULL aos seus campos de tamanho variável.
		É utilizada quando um registro é recém criado e seus campos variáveis irão passar por um realloc
	Parêmetros:
		reg = Registro que irá receber null em seus campos variáveis */
void nullFields(Registro *reg) {
    reg->razSoc  = NULL;
    reg->nomeFant = NULL;
    reg->motCanc  = NULL;
    reg->nomeEmp  = NULL;
}

/*	Descrição:
		Exibe um registro na saída padrão.
	Parêmetros:
		reg = Registro que será exibido na saída padrão */
void printReg(Registro *reg) {

	int i;

	printf("CNPJ.........: ");
	if(reg->cnpj[0] == 'n') {
		printf("null\n");
	}
	else{
		for(i=0; i<SIZE_CNPJ; i++){
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
	else{
		for(i=0; i<SIZE_DATA; i++){
			printf("%c",reg->dtReg[i]);
		}
		printf("\n");
	}

	printf("Data Canc....: ");
	if(reg->dtCanc[0] == 'n') {
		printf("null\n");
	}
	else{
		for(i=0; i<SIZE_DATA; i++){
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
		for(i=0;i<SIZE_CNPJ;i++){
			printf("%c", reg->cnpjAud[i]);
		}
		printf("\n\n");
	}
}

/*	Descrição:
		Função que compara o campo CNPJ de um registro com um valor informado
	Parêmetros:
		reg = Registro cujo campo irá sofrer a comparação
		strBusca = Valor (string) que será comparado com o campo do registro dado
	Retorno:
		Retorna o resultado da comparação, 0 para valores iguais e 1 caso contrário */
int compareFieldCNPJ(Registro *reg, char *strBusca) {
	int cmp = -1; // armazena o resultado da comparação
	int i;

	if (reg->cnpj != NULL)
		for(i=0; i<SIZE_CNPJ; i++){
			if(reg->cnpj[i] != strBusca[i]){
				return 1;
			}
		}
	return 0;
}
//----------------------------//
