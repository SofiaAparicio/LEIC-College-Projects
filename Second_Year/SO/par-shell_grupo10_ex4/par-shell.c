/*
 Par-shell - exercicio 4
80789 - Joao Pedro Pinto Silveira
81105 - Ana Sofia Aparicio da Costa
81478 - Joana de Oliveira Mira Godinho
*/

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

#include "commandlinereader.h"
#include "list.h"

#define EXIT_COMMAND "exit"
#define MAXARGS 7 /* Comando + 5 argumentos opcionais + espaco para NULL */
/*#define STATUS_INFO_LINE_SIZE 50*/
#define BUFFER_SIZE 100
#define MAXPAR 2 /*Escolher o numero maximo de processos em execução*/
#define LINE_MAX_SIZE 100

int numchildren = 0;
list_t *list;
int exit_flag = 0;
pthread_mutex_t trinco;
pthread_cond_t cond_maxproc, cond_monitor;
FILE* ficheiro;
int iteracao = -1, totaltime = 0;

/*-------------------------------------------------------------------*/
void mutex_lock(void) {
	if(pthread_mutex_lock(&trinco) != 0)
	{
		fprintf(stderr, "Error in pthread_mutex_lock()\n");
		exit(EXIT_FAILURE);
	}
}
void mutex_unlock(void) {
	if(pthread_mutex_unlock(&trinco) != 0)
	{
		fprintf(stderr, "Error in pthread_mutex_unlock()\n");
		exit(EXIT_FAILURE);
	}
}

/*-------------------------------------------------------------------*/
void cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex) {
	if(pthread_cond_wait(cond, mutex) != 0)
	{
		perror("Error in pthread_cond_wait");
		exit(EXIT_FAILURE);
	}
}
void cond_signal(pthread_cond_t *cond) {
	if(pthread_cond_signal(cond) != 0)
	{
		perror("Error in pthread_cond_signal");
		exit(EXIT_FAILURE);
	}
}
/*void cond_broadcast(pthread_cond_t *cond) {
	if(pthread_cond_broadcast(cond) != 0)
	{
		perror("Error in pthread_cond_broadcast");
		exit(EXIT_FAILURE);
	}
}*/

void *monitor() {
	int pid, status;
	time_t endtime;
	double runtime;
	while(1){
		mutex_lock();
		while(exit_flag == 0 && numchildren == 0)
			cond_wait(&cond_monitor, &trinco);
		if (numchildren == 0) {
			if (exit_flag){
				mutex_unlock();
				pthread_exit(EXIT_SUCCESS);
			}
			mutex_unlock();
		}
		else{
			/*termina processos ordeiramente e atualiza na estrutura o endtime e status*/
			mutex_unlock();
			pid = wait(&status);
			endtime = time(NULL);
			if (pid < 0) {
				if (errno == EINTR) {
					/* Este codigo de erro significa que chegou signal que interrompeu a espera 
					   pela terminacao de filho; logo voltamos a esperar */
					continue;
				}
				else {
					perror("Error waiting for child");
					exit (EXIT_FAILURE);
				}
			}
			mutex_lock();
			cond_signal(&cond_maxproc);
			runtime = update_terminated_process(list, pid, status, endtime);
			numchildren--;
			totaltime += runtime;
			fprintf(ficheiro, "iteracao %d\npid: %d execution time: %.0f s\ntotal execution time: %d s\n",
				++iteracao, pid, runtime, totaltime);
			fflush(ficheiro);
			mutex_unlock();
		}
	}
}

int main (int argc, char** argv) {

	int pid;
	time_t starttime;

	char *args[MAXARGS], linha[LINE_MAX_SIZE];

	char buffer[BUFFER_SIZE];

	pthread_t monitor_t;

	list = lst_new();

	if ( (ficheiro = fopen("log.txt", "a+")) == NULL) {
		perror("Error in fopen");
		exit(EXIT_FAILURE);
	}

	/*Leitura do ficheiro*/
	while( fgets(linha, LINE_MAX_SIZE, ficheiro) != NULL ) {

		if (sscanf(linha, "iteracao %d", &iteracao) == 1)
			;
		else
			sscanf(linha, "total execution time: %d s", &totaltime);
	}
	/*-------------------*/

	if(pthread_cond_init(&cond_maxproc, NULL) != 0) {
		perror("Error creating condition variable cond_maxproc");
		exit(EXIT_FAILURE);
	}

	if(pthread_cond_init(&cond_monitor, NULL) != 0) {
		perror("Error creating condition variable cond_monitor");
		exit(EXIT_FAILURE);
	}

	if(pthread_mutex_init(&trinco, NULL) != 0) {
		fprintf(stderr, "Error creating mutex.\n");
		exit(EXIT_FAILURE);
	}

	if (pthread_create(&monitor_t, 0, monitor, NULL) != 0) {
		fprintf(stderr, "Error creating thread.\n");
		exit(EXIT_FAILURE);
	}

	printf("Monitor thread created!\n");

	printf("Insert your commands:\n");

	while (1) {
		int numargs;

		numargs = readLineArguments(args, MAXARGS, buffer, BUFFER_SIZE);

		/* Verifica se: se chegou ao EOF (end of file) do stdin
	       ou se chegou a ordem "exit". Em ambos os casos, termina
	       ordeiramente esperando pela terminacao dos filhos */
		if (numargs <= 0)
			continue;

		if(strcmp(args[0], EXIT_COMMAND) == 0)
		{
			mutex_lock();
			exit_flag=1;
			cond_signal(&cond_monitor);
			mutex_unlock();

			if (pthread_join(monitor_t, NULL) != 0){
				fprintf(stderr, "Error joining thread.\n");
				exit(EXIT_FAILURE);
			}

			if(fclose(ficheiro) == EOF) {
				perror("Error in fclose");
				exit(EXIT_FAILURE);
			}

			lst_print(list);

			lst_destroy(list);

			if (pthread_mutex_destroy(&trinco) != 0) {
				fprintf(stderr, "Error destroying mutex.\n");
				exit(EXIT_FAILURE);
			}

			if (pthread_cond_destroy(&cond_maxproc) != 0){
				perror("Error destroying condition variable");
				exit(EXIT_FAILURE);
			}
			if (pthread_cond_destroy(&cond_monitor) != 0){
				perror("Error destroying condition variable");
				exit(EXIT_FAILURE);
			}

			exit(EXIT_SUCCESS);
		}

		mutex_lock();
		while(numchildren == MAXPAR)
			cond_wait(&cond_maxproc, &trinco);
		mutex_unlock();

		pid = fork();
		starttime = time(NULL);
		if (pid < 0) {
			perror("Failed to create new process");
			exit(EXIT_FAILURE);
		}

		if (pid > 0) { 	  /* Codigo do processo pai */
			mutex_lock();
			numchildren++;
			insert_new_process(list, pid, starttime);
			cond_signal(&cond_monitor);
			mutex_unlock();
  		}

  		else { /* Codigo do processo filho */
			if (execv(args[0], args) < 0) {
  				perror("Could not run child program. Child will exit");
  				exit(EXIT_FAILURE);
			}
  		}
  	} 
}
