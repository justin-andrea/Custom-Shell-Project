#ifndef SHELL_H
#define SHELL_H

//list of methods implemeted in shell.c
char* get_command(char* prompt);
void help();
void echo(char *lst[] , int size);
char* set();
void cd(char* path);

#endif
