#include <errno.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 

char *get_command(char *prompt) {

  char *command = "";

  while (strcmp(command, "") == 0) {
    command = readline(prompt);
  }

  return command;
}

void help() {
  printf("To type in multiple commands in one line seperate them by ;  Example: help;set;echo hello\n");
  printf("List of commands:\n");
  printf("cd: change directory\n");
  printf("echo: print to console all words after echo\n");
  printf("exit: quit the program \n");
  printf("set: set the prompt of this shell to a given character. User will be "
         "prompted to do so.\n");
  printf("schedule: input a txt file with proc times and the data for various scheduling methods will be produced\n");
	
  printf("All other commands will be executing as if it is a request to execute a program");
  printf("Written by Justin Andrea\n");
}

//method that changes directory to the specified directory
void cd(char *path) {

  errno = 0;
  
  printf("Current directory is %s\n", path);

  if (chdir(path) != 0) {
    
    perror("cd:");
  }
}
//set the prompt to the symbols given by the user
char *set() {

  char *line;

  line = readline("Input a new prompt: ");

  return line;
}

// prints to console everything written after the word "echo"
void echo(char *lst[], int size) {

  for (int i = 1; i < size; i++) {
    printf("%s ", lst[i]);
  }
  printf("\n");
}