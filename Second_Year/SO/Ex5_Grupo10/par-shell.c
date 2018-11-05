/*
 Par-shell - exercicio 5
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

#include <sys/stat.h>
#include <fcntl.h>

#include "commandlinereader.h"
#include "list.h"
#include "list_terminal.h"

#define EXIT_COMMAND "exit"
#define EXIT_GLOBAL "exit-global"
#define MAXARGS 7 /* Comando + 5 argumentos opcionais + espaco para NULL */
#define BUFFER_SIZE 100
#define MAXPAR 2 /*Escolher o numero maximo de processos em execução*/
#define LINE_MAX_SIZE 100
#define PIPE "/tmp/par-shell-in"
#define STATS "stats"
#define TERMINAL_START "/start/"
#define BUF 200
#define PERMISSOES 0777

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


/*----------------------------Monitor-----------------------------*/
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

/*----------------------Tratamento de Signal-------------------*/

int death = 0;

void morte(int s){
	death=1;
	if(close(STDIN_FILENO)==-1){
	  perror("Error closing stdin pipe");  
	}  
	if(signal(SIGINT, morte) == SIG_ERR)
	  fprintf(stderr, "Error setting signal handler.\n");
}
/*---------------------------------------------------------------*/



/*________________________________________________________________*/
/*|				MAIN				 |*/
/*|______________________________________________________________|*/

int main (int argc, char** argv) {
  
    /*------------Inicializacao de variaveis------------------*/
	FILE*	fic;
	int pid, pipe_in, pipe_out, i, fd_stdin;
	time_t starttime;

	char *args[MAXARGS], linha[LINE_MAX_SIZE];
	char buffer[BUFFER_SIZE],pipeName[BUFFER_SIZE],fileName[BUFFER_SIZE],buffer_out[BUFFER_SIZE];

	pthread_t monitor_t;

	list = lst_new();
	terminais = lst_terminal_new();

	
	if(signal(SIGINT, morte) == SIG_ERR)
	  fprintf(stderr, "Error setting signal handler.\n");
	
    /*-------------------------------------------------------*/

    
    /*--------------------Criacao dos pipes------------------*/

	unlink(PIPE);						
	
	if(mkfifo (PIPE, PERMISSOES) != 0) 			
	{
		perror("Error creating pipe");
		exit(EXIT_FAILURE);
	}
	

      /*-----------------------AbrirPipe-----------------------*/

	if ((pipe_in = open(PIPE, O_RDONLY)) == -1){
		perror("Error opening the pipe");
		exit(EXIT_FAILURE);
	}
	
	fd_stdin = dup(STDIN_FILENO);
	close(STDIN_FILENO);
	dup(pipe_in);

      /*-----------------------------------------------------*/
    
      
      /*-------Abertura/criacao e leitura do log.txt---------*/
	
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

      /*-----------------------------------------------------*/


      /*---------Criacao das variaveis de condicao-----------*/

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
      /*-----------------------------------------------------*/


      /*-------------Inicio do ciclo------------------------*/

	printf("Monitor thread created!\n");


	while (1) {
		int numargs;
		
		if(death)
		  break;

		numargs = readLineArguments(args, MAXARGS, buffer, BUFFER_SIZE);

		if(death)
		  break;
		
		if (numargs <= 1)  /*o readLine devolve sempre um argumento que e o pid*/
			continue;
		
		
		/*Tratamento sempre que um terminal inicia execucao*/
		if(strcmp(args[1],TERMINAL_START)==0){
			int pid_terminal = atoi(args[0]);
			insert_new_terminal(pid_terminal);
			continue;
		}
		
		
		/*Tratamento sempre que um terminal recebe o comando exit*/
		if(strcmp(args[1],EXIT_COMMAND)==0){
			int pid_terminal = atoi(args[0]);
			remove_terminal(pid_terminal);
			continue;
		}

		/*Tratamento sempre que um terminal recebe o comando stats*/
		if(strcmp(args[1],STATS)==0){
			sprintf(pipeName,"/tmp/pipe-%s",args[0]);
			mutex_lock();
			sprintf(buffer_out,"Number of processes: %d, totaltime: %d\n",numchildren,totaltime);
			mutex_unlock();

			if ((pipe_out= open(pipeName, O_WRONLY)) < 0){
				perror("Error opening the pipe");
				exit(EXIT_FAILURE);
			}

			if(write(pipe_out,buffer_out,BUFFER_SIZE) == -1){
			    perror("Error opening the pipe");
			    exit(EXIT_FAILURE);
			}
			close(pipe_out);
			continue;
		}

	    
		/*ciclo que retira o primeiro elemento do args (pid)*/
		for(i=0; i<numargs-1; i++){
		    args[i]=args[i+1];
		}
		args[i]=NULL;

		
		
		/* Verifica se: se chegou ao EOF (end of file) do stdin
	       ou se chegou a ordem "exit". Em ambos os casos, termina
	       ordeiramente esperando pela terminacao dos filhos */
		
		if(strcmp(args[0], EXIT_GLOBAL) == 0)
		{  
			break;
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
  			sprintf(fileName,"par-shell-out-%d.txt",getpid());
  			if  ((fic = fopen(fileName, "a+")) == NULL) {
				perror("Error in fopen");
				exit(EXIT_FAILURE);
			}
			close(STDOUT_FILENO);
			dup(fileno(fic));
			close(STDIN_FILENO);
			dup(fd_stdin);
			signal(SIGINT, SIG_IGN);

			if (execv(args[0], args) < 0) {
  				perror("Could not run child program. Child will exit");
  				exit(EXIT_FAILURE);
			}
  		}
  	}
  	
  	
  	
  	/*----------------Codigo de Saida-----------------*/
  	
  	printf("  Terminando...  \n");
	
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

	kill_em_all();

	lst_print(list);

	lst_destroy(list);

	lst_terminal_destroy();

	if(close (pipe_in) == -1){
	  perror("Error closing the pipe_in.\n");
	}
	unlink(PIPE);

	if (pthread_mutex_destroy(&trinco) != 0) {
		fprintf(stderr, "Error destroying mutex.\n");
		exit(EXIT_FAILURE);
	}

	if (pthread_cond_destroy(&cond_maxproc) != 0){
		perror("Error destroying condition variable\n");
		exit(EXIT_FAILURE);
	}
	if (pthread_cond_destroy(&cond_monitor) != 0){
		perror("Error destroying condition variable\n");
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}
