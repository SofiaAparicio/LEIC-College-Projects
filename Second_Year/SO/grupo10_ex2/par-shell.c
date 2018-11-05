/*
 Par-shell - exercicio 2
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

int numchildren = 0;
list_t *list;
int exit_flag = 0;
pthread_mutex_t trinco;

void *monitor() {
	int pid, status;
	time_t endtime;
	while(1){
		pthread_mutex_lock(&trinco);
		if (numchildren<1){
			if (exit_flag){
				pthread_mutex_unlock(&trinco);
				pthread_exit(EXIT_SUCCESS);
			}
			pthread_mutex_unlock(&trinco);
			sleep(1);
		}
		else{
			/*termina processos ordeiramente e atualiza na estrutura o endtime e status*/
			pthread_mutex_unlock(&trinco);
			pid = wait(&status);
			endtime = time(NULL);
			if (pid < 0) {
				if (errno == EINTR) {
					/* Este codigo de erro significa que chegou signal que interrompeu a espera 
					   pela terminacao de filho; logo voltamos a esperar */
					continue;
				}
				else {
					perror("Error waiting for child.");
					exit (EXIT_FAILURE);
				}
			}
			pthread_mutex_lock(&trinco);
			update_terminated_process(list, pid, status, endtime);
			numchildren--;
			pthread_mutex_unlock(&trinco);
		}
	}
}

int main (int argc, char** argv) {

	int pid;
	time_t starttime;

	char *args[MAXARGS];

	char buffer[BUFFER_SIZE];

	pthread_t monitor_t;

	pthread_mutex_init(&trinco, NULL);

	list = lst_new();

	if (pthread_create(&monitor_t, 0, monitor, NULL) != 0) {
		perror("Error creating monitor thread. Program exiting...\n");
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
			pthread_mutex_lock(&trinco);
			exit_flag=1;
			pthread_mutex_unlock(&trinco);

			pthread_join(monitor_t, NULL);

			lst_print(list);

			lst_destroy(list);

			pthread_mutex_destroy(&trinco);
	    
		    exit(EXIT_SUCCESS);
		}
    
		pid = fork();
		starttime = time(NULL);
		if (pid < 0) {
			perror("Failed to create new process.");
			exit(EXIT_FAILURE);
		}

		if (pid > 0) { 	  /* Codigo do processo pai */
			pthread_mutex_lock(&trinco);
			numchildren ++;
			insert_new_process(list, pid, starttime);
			pthread_mutex_unlock(&trinco);
  		}

  		else { /* Codigo do processo filho */
			if (execv(args[0], args) < 0) {
  				perror("Could not run child program. Child will exit.");
  				exit(EXIT_FAILURE);
			}
  		}
  	} 
}
