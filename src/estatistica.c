/*
GRUPO G5
Felipe Scrochio Custodio, 9442688
Felipe Contini Torres, 9253670
Júlia Diniz Ferreira, 9364865
Bruno Henrique Rasteiro, 9292910
*/

void listarRemovidos(FILE *file) {
	
	long int head;
	long int offset;

	int regCounter = 0;
	int size;
	long int next;

	// salva posicao atual do arquivo para o usuario nao notar as alteracoes feitas
	long int filePos = ftell(file);

	// fseek para o inicio do arquivo
	fseek(file, 0, SEEK_SET);
	
	/* FORMATO DA EXCLUSAO LOGICA

	fwrite(&exc_log, sizeof(char), 1, file);	//caracter de exclusão lógica
	fwrite(&sizeReg, sizeof(int), 1, file);		// tamanho do registro
	fwrite(&head, sizeof(long int), 1, file);	// byte offset do próximo 
												// registro removido (antiga 
												// cabeça da lista)

	*/

	// le a cabeça da lista de removidos (byte offset do primeiro registro removido)
	// se for -1 está vazio
	fread(&head, sizeof(long int), 1, file);
	if(head == -1){

		fprintf(stderr, "Lista de registros removidos está vazia.\n");
		
		// Retorna o indice do arquivo para onde o usuario estava originalmente
		fseek(file, filePos, SEEK_SET);
		return;
	}

	// Move para o primeiro registro removido + sizeof(char) do caractere de
	// exclusao + sizeof(int) do tamanho do registro
	fseek(file, head + sizeof(char), SEEK_SET);

    // Lê do arquivo até acabar a lista de registros removidos
	do {
		
		// Incrementa o contador de registros
		regCounter++;
		
		// Le o tamanho do registro
		fread(&size, sizeof(int), file);

		// Imprime o byte offset do registro e seu tamanho
		fprintf(stderr, "Offset: %-6ld\t Tamanho: %-4d\n", ftell(file), size);
		
		// Le o byte offset do próximo registro
		fread(&next, sizeof(long int), file);

		// vai pro próximo da lista
		fseek(file, next, SEEK_SET);

	} while(next != -1);

	fprintf(stderr, "Numero de registros removidos: %d\n", regCounter);

	// Retorna o indice do arquivo para onde o usuario estava originalmente
	fseek(file, filePos, SEEK_SET);
}

int quant_entradas(){
	
}