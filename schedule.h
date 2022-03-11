#ifndef SCHEDULE_H
#define SCHEDULE_H


typedef struct proc {  
    int id;
    int length;
    int time_wait;
    int time_turnaround;
    int time_response;
	int tickets;
	int complete;
}proc;

//list of methods

void fifo(proc list[], int size);
void sjf(proc list[], int size);
void roundRobin(proc list[], int size);
void lottery(proc list[], int size);

#endif