#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//includes for fork() and other system calls
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <readline/readline.h>

#include "shell.h"
#include "schedule.h"


#define MAX_LINE_LENGTH 80

// This program replicates a shell similar to those found in unix
// There are five built in calls: cd, echo, exit, help, and set
// The rest of use inputed commands will be treated as an execution of a program
int main(int argc, char *argv[]) {

  char *prompt = "shell> ";
  char *command;

  while (1) {

    command = get_command(prompt);
	//copy command so we have command that is unaffected by strtok
    char *copy_command = malloc(strlen(command) + 1);
    strcpy(copy_command, command);

	
    char *token, *separator = ";";
	//keeps size of token entity
    int count = 0;
	
	//tokenize the command by ;
    token = strtok(command, separator);
	
	//get amount of tokens
    while (token != NULL) {
      count += 1;
      token = strtok(NULL, separator);
    }

    // create and fill array to store tokens
    char *arr[count + 1]; // +1 to store extra null space at the end
    token = strtok(copy_command, separator);
    int loop = 0;
    while (token != NULL) {
      arr[loop++] = token;
      token = strtok(NULL, separator);
    }
    arr[loop] = NULL;

    // tokenize our input string to isolate the keyword to match
    // now itereate through the list of commands one by one

    for (int i = 0; i < count; i++) {
      
      char *lst_commands = malloc(strlen(arr[i]) + 1);
      strcpy(lst_commands, arr[i]);

      
      separator = " ";
      int count2 = 0;

      char *arr2[512];
	  
	  //tokenize by space
      token = strtok(lst_commands, separator);

      while (token != NULL) {
		  
        arr2[count2++] = token;
        token = strtok(NULL, separator);
      }
	  //add null so it can identify the end of the list
      arr2[count2] = NULL;

	  //chain of if statements that direct the input to the corret handling method
	  //example: if command is help, execute help()
      if (strcmp("help", arr2[0]) == 0) {
        help();
	  } else if(strcmp("schedule", arr2[0]) == 0){
		//  read file in 
		// from https://www.programiz.com/c-programming/c-file-input-output
		// and https://riptutorial.com/c/example/29223/read-lines-from-a-file
		char* name = arr2[1];
		int num;
		char* curLine = "";
		FILE *file;
		char line[MAX_LINE_LENGTH] = {0};
		int size;
		
		if((file = fopen(name,"r")) == NULL){
			printf("file name not recognized \n");
			exit(0);		
		}
		
		//read the file to get the number of procs being entered
		while(fgets(line, MAX_LINE_LENGTH, file )){
			size++;
		}
		//create an array of procs with that size
		proc procs[size];
		
		
		//close and reopen file so you can actual get the data into an array of procs
		fclose(file);
		file = fopen(name,"r");

		
		int count = 0;
		while(fgets(line, MAX_LINE_LENGTH, file )){
			
			
			curLine = line;
			num = atoi(curLine);
			procs[count].id = count;
			procs[count].length = num;
			procs[count].time_wait = 0;
			procs[count].time_response = 0;
			procs[count].time_turnaround = 0;
			procs[count].tickets = num;
			procs[count].complete = 0;

			count++;
		}
		
		for(int i = 0; i < size; i++){
			
		
		}
		fclose(file);
		
		
		//create arrays for round robin and lottery so we don't have to worry about ruining the data of a single array
		proc procRR[size];
		proc procLot[size];
		
		for(int a = 0; a <size;a++){
			procRR[a].length = procs[a].length;
			procLot[a].length = procs[a].length;
			procLot[a].tickets = procs[a].tickets;
			procLot[a].complete = 0;
		}

		fifo(procs,size);
		
		sjf(procs,size);
		
		roundRobin(procRR,size);
		
		lottery(procLot,size);

      } else if (strcmp("set", arr2[0]) == 0) {
        prompt = set();
      } else if (strcmp("cd", arr2[0]) == 0) {
        cd(arr2[1]);
      } else if (strcmp("echo", arr2[0]) == 0) {
        echo(arr2, count2);
      } else if (strcmp("quit", arr2[0]) == 0 || strcmp("exit", arr2[0]) == 0) {
        exit(0);
      } else {
		
		//taken from creat_proc.c
        pid_t pid;

        pid = fork();

        if (pid < 0) { /* error occurred */
          fprintf(stderr, "fork() failed\n");
          return 1;
        } else if (pid == 0) { /* child process */
		  // execute command using execvp
          execvp(arr2[0], arr2);
        } else { /* parent process */
          /* parent will wait for the child to complete */
          wait(NULL);
        }
      }
	  //free the malloced space
      free(lst_commands);
    }
    // free the malloced space
    free(copy_command);
  }

  return EXIT_SUCCESS;
}