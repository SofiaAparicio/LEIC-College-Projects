/*
 Par-shell - exercicio 3
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
#include <semaphore.h>

#include "commandlinereader.h"
#include "list.h"

#define EXIT_COMMAND "exit"
#define MAXARGS 7 /* Comando + 5 argumentos opcionais + espaco para NULL */
/*#define STATUS_INFO_LINE_SIZE 50*/
#define BUFFER_SIZE 100
#define MAXPAR 5 /*Escolher o numero maximo de processos em execução*/

int numchildren = 0;
list_t *list;
int exit_flag = 0;
pthread_mutex_t trinco;
sem_t sem_maxproc, sem_monitor;

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
void esperar(sem_t *semaf) {
	if(sem_wait(semaf) != 0)
	{
		perror("Error in sem_wait()\n");
		exit(EXIT_FAILURE);
	}
}
void assinalar(sem_t *semaf) {
	if(sem_post(semaf) != 0)
	{
		perror("Error in sem_post()\n");
		exit(EXIT_FAILURE);
	}
}

void *monitor() {
	int pid, status;
	time_t endtime;
	while(1){
		esperar(&sem_monitor);
		mutex_lock();
		if (exit_flag && numchildren == 0){
			mutex_unlock();
			pthread_exit(EXIT_SUCCESS);
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
					assinalar(&sem_monitor);
					continue;
				}
				else {
					perror("Error waiting for child.");
					exit (EXIT_FAILURE);
				}
			}
			assinalar(&sem_maxproc);
			mutex_lock();
			update_terminated_process(list, pid, status, endtime);
			numchildren--;
			mutex_unlock();
		}
	}
}

int main (int argc, char** argv) {

	int pid;
	time_t starttime;

	char *args[MAXARGS];

	char buffer[BUFFER_SIZE];

	pthread_t monitor_t;

	list = lst_new();

	if(sem_init(&sem_maxproc, 0, MAXPAR) != 0) {
		perror("Error creating semaphore.\n");
		exit(EXIT_FAILURE);
	}

	if(sem_init(&sem_monitor, 0, 0) != 0) {
		perror("Error creating semaphore.\n");
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
			mutex_unlock();

			assinalar(&sem_monitor);

			if (pthread_join(monitor_t, NULL) != 0){
				fprintf(stderr, "Error joing thread.\n");
				exit(EXIT_FAILURE);
			}

			lst_print(list);

			lst_destroy(list);

			if (pthread_mutex_destroy(&trinco) != 0) {
				fprintf(stderr, "Error destroying mutex.\n");
				exit(EXIT_FAILURE);
			}

			if (sem_destroy(&sem_maxproc) != 0){
				perror("Error destroying semaphore");
				exit(EXIT_FAILURE);
			}
			if (sem_destroy(&sem_monitor) != 0){
				perror("Error destroying semaphore");
				exit(EXIT_FAILURE);
			}
	    
		    exit(EXIT_SUCCESS);
		}

		esperar(&sem_maxproc);
    
		pid = fork();
		starttime = time(NULL);
		if (pid < 0) {
			perror("Failed to create new process.");
			exit(EXIT_FAILURE);
		}

		if (pid > 0) { 	  /* Codigo do processo pai */
			mutex_lock();
			numchildren++;
			insert_new_process(list, pid, starttime);
			mutex_unlock();
			assinalar(&sem_monitor);
  		}

  		else { /* Codigo do processo filho */
			if (execv(args[0], args) < 0) {
  				perror("Could not run child program. Child will exit.");
  				exit(EXIT_FAILURE);
			}
  		}
  	} 
}
