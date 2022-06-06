#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "stdio.h"
#include "errno.h"
#include "stdlib.h"
#include "unistd.h"
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <signal.h>


void sig_handler(int s)
{
	printf("\nYou typed Control-C!\n");	
}

int main() {
	
	struct sigaction sigIntHandler;
  	sigIntHandler.sa_handler = sig_handler;
  	sigemptyset(&sigIntHandler.sa_mask);
  	sigIntHandler.sa_flags = 0;
  	sigaction(SIGINT, &sigIntHandler, NULL);
	
	char command[1024] ,last_command[1024], temp_command[1024];
	char *token;
	int i;
	char *outfile, *infile;
	int fd, amper, redirect_append, redirect_out, redirect_in, piping, retid, argc1, status;
	int fildes[2];
	char *argv1[10], *argv2[10];
	char prompt[100];
	strcpy(prompt,"hello");
		
	while (1)
	{
    		printf("%s: ", prompt);    		
    		fgets(command, 1024, stdin);
    		command[strlen(command) - 1] = '\0';
    		strcpy(temp_command, command);
    		piping = 0;

    		/* parse command line */
    		i = 0;
    		token = strtok (command," ");
    		while (token != NULL)
    		{
        		argv1[i] = token;
        		token = strtok (NULL, " ");
        		i++;
        		if (token && ! strcmp(token, "|")) {
            			piping = 1;
            			break;
        		}
    		}
   		argv1[i] = NULL;
    		argc1 = i;

		if (argv1[0] == NULL)   		// if is command empty 
			continue;

	    	if (piping) {               	    	//  command whitout contain pipe 
			i = 0;
			while (token!= NULL)
			{
		    		token = strtok (NULL, " ");
		    		argv2[i] = token;
		    		i++;
			}
			argv2[i] = NULL;
	    	}

	    	if (! strcmp(argv1[argc1 - 1], "&")) {        // Does command line end with & 
			amper = 1;
			argv1[argc1 - 1] = NULL;
			}
	    	else 
			amper = 0; 

	   	if (argc1 == 3 && ! strcmp(argv1[0], "prompt") && ! strcmp(argv1[1], "=")) {      //for change prompt
			strcpy(prompt, argv1[2]);
			continue;
		}	
	    	if (argc1 == 1 && ! strcmp(argv1[0], "quit")) {        //for quiting shell
			exit(0);
		}
	     	if (argc1 == 2 && ! strcmp(argv1[0], "cd")) {         //for changing directory
			chdir(argv1[1]);
			continue;
		}
	    	if (argc1 == 2 && ! strcmp(argv1[0], "echo")) {    //for printing last command status

			if (! strcmp(argv1[1], "$?")){
				printf("%d\n", status);
				continue;
			}
		}
		if (! strcmp(argv1[0], "!!")){
				printf("%s\n", last_command);
				continue;
			}
		if (argc1 > 1 && ! strcmp(argv1[argc1 - 2], ">")) {      	//for redirect stdout
		redirect_out = 1;
		argv1[argc1 - 2] = NULL;
		outfile = argv1[argc1 - 1];
		}else {
			redirect_out = 0; 
		}
		if (argc1 > 1 && ! strcmp(argv1[argc1 - 2], "<")) {         	// for redirect stdin
			printf("is printing2\n");
			redirect_in = 1;
			argv1[argc1 - 2] = NULL;
			infile = argv1[argc1 - 1];
			printf("%s\n", infile);
		}else {
			redirect_in = 0; 
		}		
		if (argc1 > 1 && ! strcmp(argv1[argc1 - 2], ">>")) {        //for redirect out and append
			redirect_append = 1;
			argv1[argc1 - 2] = NULL;
			outfile = argv1[argc1 - 1];
		}else {
			redirect_append = 0; 
		}

		// for commands not part of the shell command language 

		if (fork() == 0) { 
		// redirection of IO ? 
		signal(SIGINT, SIG_DFL);           //return to defult behivor
		if (redirect_out) {
				fd = creat(outfile, 0660); 
				close (STDOUT_FILENO) ; 
				dup(fd); 
				close(fd); 
		} 	
		if (redirect_in) {
			printf("is printing1\n");
				fd = open(infile, O_RDONLY); 
				close (STDIN_FILENO) ; 
				dup(fd); 
				close(fd); 
		} 
		if (redirect_append) {
			FILE *f = fopen(outfile, "a");
			fd = fileno(f);
				close (STDOUT_FILENO) ; 
				dup(fd); 
				close(fd); 
		} 	
		if (piping) {
				pipe (fildes);
				if (fork() == 0) { 
					close(STDOUT_FILENO); 
					dup(fildes[1]); 
					close(fildes[1]); 
					close(fildes[0]); 
					execvp(argv1[0], argv1);
				} 
				close(STDIN_FILENO);
				dup(fildes[0]);
				close(fildes[0]); 
				close(fildes[1]); 
				execvp(argv2[0], argv2);
		}else
				execvp(argv1[0], argv1);
		}
		// parent continues over here... 
		// waits for child to exit if required 
		if (amper == 0)
		retid = wait(&status);
		strcpy(last_command, temp_command);
	}
}
