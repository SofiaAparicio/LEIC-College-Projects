/*
80789 - Joao Pedro Pinto Silveira
81105 - Ana Sofia Aparicio da Costa
81478 - Joana de Oliveira Mira Godinho
*/

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <wait.h>
#include "commandlinereader.h"

/*
Estrutura que representa um processo e guarda o seu estado e ID
*/
typedef struct {
	int status, p_id;
} process;

int main(){

	char* command[7];
	int pid, forkflag = 0, i = 0; //forkflag -> conta o numero de filhos criados
	process* p_vec;
	while(readLineArguments(command,7) < 1) //Quando devolve 0 (nao tem argumentos para ler) ou -1 (caso de erro)
		printf("Erro na leitura do comando, tente novamente\n");
	
	while (strcmp(command[0],"exit")){
		pid = fork();
		forkflag++;
		if (pid == -1) {
			perror("Erro na criacao de processo: ");
			continue;
		}
		if (pid == 0) {
			execv(command[0], command); //Executa no filho o programa introduzido na shell
			perror("Erro do exec: ");
			exit(EXIT_FAILURE);
		}

		while(readLineArguments(command,7) < 1) //Quando devolve 0 (nao tem argumentos para ler) ou -1 (caso de erro)
			printf("Erro na leitura do comando, tente novamente\n");
	}

	//Aloca memoria para um vetor de processos com tamanho igual ao numero de filhos
	p_vec = (process*) malloc(sizeof(process)*forkflag);

	//Faz tantos waits como o numero de filhos
	for(i=0;i<forkflag;i++)
		p_vec[i].p_id = wait(&(p_vec[i].status));

	//Imprime o ID e o estado de cada filho
	for(i=0;i<forkflag;i++){
		if (WIFEXITED(p_vec[i].status))
			printf("O processo com ID: %d devolveu: %d\n", p_vec[i].p_id, WEXITSTATUS(p_vec[i].status));
		else
			printf("O processo com ID: %d terminou irregularmente\n", p_vec[i].p_id);
	}

	free(p_vec); //liberta a memoria alocada para o vetor de processos

	return 0;
}