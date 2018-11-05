#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "commandlinereader.h"
#include "list.h"

#define TAMMSG  1000
#define BUF 100
#define MAXARGS 7
#define BUFFER_SIZE 100
#define STATS "stats\n"
#define EXIT_COMMAND "exit\n"
#define PERMISSOES 0777

char buffer[BUFFER_SIZE], *args[MAXARGS],buffer_out[BUFFER_SIZE];
int f_write , f_read;

void writePipe(){
  char msg[BUFFER_SIZE];


  sprintf(msg,"%d ",getpid());
  strcat(msg,buffer);

  if(write(f_write,msg,strlen(msg)) < 0){
    perror("Erro writing in pipe");
    exit(EXIT_FAILURE);
  }
  
}



int main (int argc, char** argv) {
    char pipeName[BUFFER_SIZE];
    sprintf(buffer,"/start/\n");

   while((f_write = open(argv[1], O_WRONLY)) < 0){
        continue;
      }

      writePipe();

    printf("Insert your commands:\n");

    while(1){

      if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
          perror("Error in fgets");
          exit(EXIT_FAILURE);
      }


      if(strcmp(buffer,STATS)==0){
        sprintf(pipeName,"/tmp/pipe-%d",getpid());
	
	if(mkfifo (pipeName, PERMISSOES) < 0) 	
	{
	    perror("Error creating the pipe");
	    exit(EXIT_FAILURE);
	}
	
	writePipe();
	
        if((f_read = open(pipeName, O_RDONLY)) < 0){
	  perror("Error opening the pipe");
	  exit(EXIT_FAILURE);
	} 
        
        if(read(f_read,buffer_out,BUFFER_SIZE) == -1){
          perror("Error opening the pipe");
          exit(EXIT_FAILURE);
        }
        close(f_read);
        printf("%s",buffer_out);
        unlink(pipeName);
      }
      else{
	writePipe();
	
	if(strcmp(buffer,EXIT_COMMAND)==0){
	  exit(EXIT_SUCCESS);
	}
      }
    
      
    }
}
